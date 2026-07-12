// Ground Truth-based Ambient Occlusion (GTAO)
// Paper: https://www.activision.com/cdn/research/Practical_Real_Time_Strategies_for_Accurate_Indirect_Occlusion_NEW%20VERSION_COLOR.pdf
// Presentation: https://blog.selfshadow.com/publications/s2016-shading-course/activision/s2016_pbs_activision_occlusion.pdf
//
// Ported from Bevy Engine, crates/bevy_pbr/src/ssao/gtao.wgsl (v0.13.2), licensed
// MIT OR Apache-2.0 (see res/licenses/), itself heavily based on XeGTAO v1.30 from Intel (MIT):
// https://github.com/GameTechDev/XeGTAO/blob/0d177ce06bfa642f64d8af4de1197ad1bcb862d4/Source/Rendering/Shaders/XeGTAO.hlsli
//
// PORT:
// - Bevy view/globals bindings -> the mod's own uniform block (matrices from Dusklight's
//   CameraService, WebGPU clip convention, reversed-Z - the same convention Bevy uses).
// - Prepass normals -> normals reconstructed from depth (atyuwen's accurate 5-tap method,
//   https://atyuwen.github.io/posts/normal-reconstruction/).
// - Sampler-based reads -> textureLoad (r32float is unfilterable without optional features);
//   the mip level for the XeGTAO bandwidth optimization is selected explicitly per load.
// - effect_radius and slice/sample counts come from uniforms instead of constants/shader defs
//   (game world units are ~100x larger than Bevy's meters, and quality is a live setting).
// - No TEMPORAL_JITTER: the noise index is pinned (no TAA; the spatial denoiser is the only
//   filter, a configuration XeGTAO supports).
// - Storage format r16float -> r32float (core WebGPU storage format).

struct Uniforms {
    projection: mat4x4f,
    inverse_projection: mat4x4f,
    size: vec2f,
    inv_size: vec2f,
    depth_scale: vec2f,
    effect_radius: f32,
    intensity: f32,
    slice_count: f32,
    samples_per_slice_side: f32,
    debug_view: u32,
    _pad: f32,
}

@group(0) @binding(0) var preprocessed_depth: texture_2d<f32>;
@group(0) @binding(1) var hilbert_index_lut: texture_2d<u32>;
@group(0) @binding(2) var ambient_occlusion: texture_storage_2d<r32float, write>;
@group(0) @binding(3) var depth_differences: texture_storage_2d<r32uint, write>;
@group(0) @binding(4) var<uniform> uniforms: Uniforms;

const PI: f32 = 3.141592653589793;
const HALF_PI: f32 = 1.5707963267948966;

fn fast_sqrt(x: f32) -> f32 {
    return bitcast<f32>(0x1fbd1df5 + (bitcast<i32>(x) >> 1u));
}

fn fast_acos(in_x: f32) -> f32 {
    let x = abs(in_x);
    var res = -0.156583 * x + HALF_PI;
    res *= fast_sqrt(1.0 - x);
    return select(PI - res, res, in_x >= 0.0);
}

fn load_noise(pixel_coordinates: vec2<i32>) -> vec2<f32> {
    let index = textureLoad(hilbert_index_lut, pixel_coordinates % 64, 0).r;
    // R2 sequence - http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences
    return fract(0.5 + f32(index) * vec2<f32>(0.75487766624669276005, 0.5698402909980532659114));
}

fn load_depth(pixel_coordinates: vec2<i32>, mip_level: i32) -> f32 {
    let mip_size = max(vec2<i32>(uniforms.size) >> vec2<u32>(u32(mip_level)), vec2<i32>(1i));
    let coordinates = clamp(pixel_coordinates, vec2<i32>(0i), mip_size - 1i);
    return textureLoad(preprocessed_depth, coordinates, mip_level).r;
}

// Calculate differences in depth between neighbor pixels (later used by the spatial denoiser pass to preserve object edges)
fn calculate_neighboring_depth_differences(pixel_coordinates: vec2<i32>) -> f32 {
    // Sample the pixel's depth and 4 depths around it
    // PORT: explicit loads instead of two textureGathers.
    let depth_center = load_depth(pixel_coordinates, 0i);
    let depth_left = load_depth(pixel_coordinates + vec2<i32>(-1i, 0i), 0i);
    let depth_top = load_depth(pixel_coordinates + vec2<i32>(0i, -1i), 0i);
    let depth_bottom = load_depth(pixel_coordinates + vec2<i32>(0i, 1i), 0i);
    let depth_right = load_depth(pixel_coordinates + vec2<i32>(1i, 0i), 0i);

    // Calculate the depth differences (large differences represent object edges)
    var edge_info = vec4<f32>(depth_left, depth_right, depth_top, depth_bottom) - depth_center;
    let slope_left_right = (edge_info.y - edge_info.x) * 0.5;
    let slope_top_bottom = (edge_info.w - edge_info.z) * 0.5;
    let edge_info_slope_adjusted = edge_info + vec4<f32>(slope_left_right, -slope_left_right, slope_top_bottom, -slope_top_bottom);
    edge_info = min(abs(edge_info), abs(edge_info_slope_adjusted));
    let bias = 0.25; // Using the bias and then saturating nudges the values a bit
    let scale = depth_center * 0.011; // Weight the edges by their distance from the camera
    edge_info = saturate((1.0 + bias) - edge_info / scale); // Apply the bias and scale, and invert edge_info so that small values become large, and vice versa

    // Pack the edge info into the texture
    let edge_info_packed = vec4<u32>(pack4x8unorm(edge_info), 0u, 0u, 0u);
    textureStore(depth_differences, pixel_coordinates, edge_info_packed);

    return depth_center;
}

fn reconstruct_view_space_position(depth: f32, uv: vec2<f32>) -> vec3<f32> {
    let clip_xy = vec2<f32>(uv.x * 2.0 - 1.0, 1.0 - 2.0 * uv.y);
    let t = uniforms.inverse_projection * vec4<f32>(clip_xy, depth, 1.0);
    let view_xyz = t.xyz / t.w;
    return view_xyz;
}

fn view_position_at(pixel_coordinates: vec2<i32>) -> vec3<f32> {
    let depth = load_depth(pixel_coordinates, 0i);
    let uv = (vec2<f32>(pixel_coordinates) + 0.5) * uniforms.inv_size;
    return reconstruct_view_space_position(depth, uv);
}

// PORT: replaces Bevy's load_normal_view_space (which reads a prepass normal texture we do
// not have). Accurate view-space normal reconstruction from depth, atyuwen's 5-tap method:
// for each axis, extrapolate the center depth from the two taps on each side and derive the
// tangent from whichever side predicts it better. This keeps normals stable across depth
// discontinuities where naive derivatives smear.
fn reconstruct_normal(pixel_coordinates: vec2<i32>, pixel_position: vec3<f32>, depth_center: f32) -> vec3<f32> {
    let depth_left1 = load_depth(pixel_coordinates + vec2<i32>(-1i, 0i), 0i);
    let depth_left2 = load_depth(pixel_coordinates + vec2<i32>(-2i, 0i), 0i);
    let depth_right1 = load_depth(pixel_coordinates + vec2<i32>(1i, 0i), 0i);
    let depth_right2 = load_depth(pixel_coordinates + vec2<i32>(2i, 0i), 0i);
    let depth_top1 = load_depth(pixel_coordinates + vec2<i32>(0i, -1i), 0i);
    let depth_top2 = load_depth(pixel_coordinates + vec2<i32>(0i, -2i), 0i);
    let depth_bottom1 = load_depth(pixel_coordinates + vec2<i32>(0i, 1i), 0i);
    let depth_bottom2 = load_depth(pixel_coordinates + vec2<i32>(0i, 2i), 0i);

    let use_left = abs(2.0 * depth_left1 - depth_left2 - depth_center) <
        abs(2.0 * depth_right1 - depth_right2 - depth_center);
    let use_top = abs(2.0 * depth_top1 - depth_top2 - depth_center) <
        abs(2.0 * depth_bottom1 - depth_bottom2 - depth_center);

    var ddx: vec3<f32>;
    if use_left {
        ddx = pixel_position - view_position_at(pixel_coordinates + vec2<i32>(-1i, 0i));
    } else {
        ddx = view_position_at(pixel_coordinates + vec2<i32>(1i, 0i)) - pixel_position;
    }
    var ddy: vec3<f32>;
    if use_top {
        ddy = pixel_position - view_position_at(pixel_coordinates + vec2<i32>(0i, -1i));
    } else {
        ddy = view_position_at(pixel_coordinates + vec2<i32>(0i, 1i)) - pixel_position;
    }

    var normal = normalize(cross(ddy, ddx));
    // Guard the orientation: the normal must face the camera.
    if dot(normal, pixel_position) > 0.0 {
        normal = -normal;
    }
    return normal;
}

fn load_and_reconstruct_view_space_position(uv: vec2<f32>, sample_mip_level: f32) -> vec3<f32> {
    // PORT: point-sample the selected mip explicitly instead of textureSampleLevel.
    let mip_level = i32(sample_mip_level + 0.5);
    let mip_size = max(vec2<i32>(uniforms.size) >> vec2<u32>(u32(mip_level)), vec2<i32>(1i));
    let depth = load_depth(vec2<i32>(uv * vec2<f32>(mip_size)), mip_level);
    return reconstruct_view_space_position(depth, uv);
}

@compute
@workgroup_size(8, 8, 1)
fn gtao(@builtin(global_invocation_id) global_id: vec3<u32>) {
    let slice_count = uniforms.slice_count;
    let samples_per_slice_side = uniforms.samples_per_slice_side;
    let effect_radius = uniforms.effect_radius;
    let falloff_range = 0.615 * effect_radius;
    let falloff_from = effect_radius * (1.0 - 0.615);
    let falloff_mul = -1.0 / falloff_range;
    let falloff_add = falloff_from / falloff_range + 1.0;

    let pixel_coordinates = vec2<i32>(global_id.xy);
    let uv = (vec2<f32>(pixel_coordinates) + 0.5) * uniforms.inv_size;

    var pixel_depth = calculate_neighboring_depth_differences(pixel_coordinates);
    let raw_depth = pixel_depth;
    pixel_depth += 0.00001; // Avoid depth precision issues

    let pixel_position = reconstruct_view_space_position(pixel_depth, uv);
    // PORT: the reconstruction differences the center position against neighbor positions
    // built from unbiased depths, so its center must use the raw depth too: at this game's
    // depth scale (far plane 200000 -> depth ~5e-3) Bevy's +0.00001 bias is comparable to a
    // one-pixel depth step, and a biased center corrupts both tangents.
    let pixel_normal = reconstruct_normal(
        pixel_coordinates, reconstruct_view_space_position(raw_depth, uv), raw_depth);
    let view_vec = normalize(-pixel_position);

    let noise = load_noise(pixel_coordinates);
    let sample_scale = (-0.5 * effect_radius * uniforms.projection[0][0]) / pixel_position.z;

    var visibility = 0.0;
    for (var slice_t = 0.0; slice_t < slice_count; slice_t += 1.0) {
        let slice = slice_t + noise.x;
        let phi = (PI / slice_count) * slice;
        let omega = vec2<f32>(cos(phi), sin(phi));

        let direction = vec3<f32>(omega.xy, 0.0);
        let orthographic_direction = direction - (dot(direction, view_vec) * view_vec);
        let axis = cross(direction, view_vec);
        let projected_normal = pixel_normal - axis * dot(pixel_normal, axis);
        let projected_normal_length = length(projected_normal);

        let sign_norm = sign(dot(orthographic_direction, projected_normal));
        let cos_norm = saturate(dot(projected_normal, view_vec) / projected_normal_length);
        let n = sign_norm * fast_acos(cos_norm);

        let min_cos_horizon_1 = cos(n + HALF_PI);
        let min_cos_horizon_2 = cos(n - HALF_PI);
        var cos_horizon_1 = min_cos_horizon_1;
        var cos_horizon_2 = min_cos_horizon_2;
        let sample_mul = vec2<f32>(omega.x, -omega.y) * sample_scale;
        for (var sample_t = 0.0; sample_t < samples_per_slice_side; sample_t += 1.0) {
            var sample_noise = (slice_t + sample_t * samples_per_slice_side) * 0.6180339887498948482;
            sample_noise = fract(noise.y + sample_noise);

            var s = (sample_t + sample_noise) / samples_per_slice_side;
            s *= s; // https://github.com/GameTechDev/XeGTAO#sample-distribution
            let sample = s * sample_mul;

            // * uniforms.size gets us from [0, 1] to [0, viewport_size], which is needed for this to get the correct mip levels
            let sample_mip_level = clamp(log2(length(sample * uniforms.size)) - 3.3, 0.0, 4.0); // https://github.com/GameTechDev/XeGTAO#memory-bandwidth-bottleneck
            let sample_position_1 = load_and_reconstruct_view_space_position(uv + sample, sample_mip_level);
            let sample_position_2 = load_and_reconstruct_view_space_position(uv - sample, sample_mip_level);

            let sample_difference_1 = sample_position_1 - pixel_position;
            let sample_difference_2 = sample_position_2 - pixel_position;
            let sample_distance_1 = length(sample_difference_1);
            let sample_distance_2 = length(sample_difference_2);
            var sample_cos_horizon_1 = dot(sample_difference_1 / sample_distance_1, view_vec);
            var sample_cos_horizon_2 = dot(sample_difference_2 / sample_distance_2, view_vec);

            let weight_1 = saturate(sample_distance_1 * falloff_mul + falloff_add);
            let weight_2 = saturate(sample_distance_2 * falloff_mul + falloff_add);
            sample_cos_horizon_1 = mix(min_cos_horizon_1, sample_cos_horizon_1, weight_1);
            sample_cos_horizon_2 = mix(min_cos_horizon_2, sample_cos_horizon_2, weight_2);

            cos_horizon_1 = max(cos_horizon_1, sample_cos_horizon_1);
            cos_horizon_2 = max(cos_horizon_2, sample_cos_horizon_2);
        }

        let horizon_1 = fast_acos(cos_horizon_1);
        let horizon_2 = -fast_acos(cos_horizon_2);
        let v1 = (cos_norm + 2.0 * horizon_1 * sin(n) - cos(2.0 * horizon_1 - n)) / 4.0;
        let v2 = (cos_norm + 2.0 * horizon_2 * sin(n) - cos(2.0 * horizon_2 - n)) / 4.0;
        visibility += projected_normal_length * (v1 + v2);
    }
    visibility /= slice_count;
    visibility = clamp(visibility, 0.03, 1.0);

    textureStore(ambient_occlusion, pixel_coordinates, vec4<f32>(visibility, 0.0, 0.0, 0.0));
}
