// Inputs a depth texture and outputs a MIP-chain of depths.
//
// Because SSAO's performance is bound by texture reads, this increases
// performance over using the full resolution depth for every sample.
//
// Reference: https://research.nvidia.com/sites/default/files/pubs/2012-06_Scalable-Ambient-Obscurance/McGuire12SAO.pdf, section 2.2
//
// Ported from Bevy Engine, crates/bevy_pbr/src/ssao/preprocess_depth.wgsl (v0.13.2),
// licensed MIT OR Apache-2.0 (see res/licenses/), itself derived from Intel XeGTAO (MIT).
//
// PORT: sampler-based gathers replaced with textureLoad (r32float is not filterable without
// optional device features), Bevy view uniforms replaced with the mod's own uniform block,
// storage format r16float -> r32float (core WebGPU storage format). MIP 4 moved into its own
// entry point (core WebGPU limit is 4 storage textures per stage).

struct Uniforms {
    projection: mat4x4f,
    inverse_projection: mat4x4f,
    size: vec2f,        // AO chain size in pixels (MIP 0 of the preprocessed depth)
    inv_size: vec2f,
    depth_scale: vec2f, // input depth snapshot pixels per chain pixel (1 or 2)
    effect_radius: f32, // view-space units
    intensity: f32,
    slice_count: f32,
    samples_per_slice_side: f32,
    debug_view: u32,
    _pad: f32,
}

@group(0) @binding(0) var input_depth: texture_2d<f32>;
@group(0) @binding(1) var preprocessed_depth_mip0: texture_storage_2d<r32float, write>;
@group(0) @binding(2) var preprocessed_depth_mip1: texture_storage_2d<r32float, write>;
@group(0) @binding(3) var preprocessed_depth_mip2: texture_storage_2d<r32float, write>;
@group(0) @binding(4) var preprocessed_depth_mip3: texture_storage_2d<r32float, write>;
@group(0) @binding(5) var<uniform> uniforms: Uniforms;
// downsample_mip4 entry point only (disjoint subresources of the same texture).
@group(0) @binding(6) var preprocessed_depth_mip3_in: texture_2d<f32>;
@group(0) @binding(7) var preprocessed_depth_mip4: texture_storage_2d<r32float, write>;

// PORT: replaces the textureGather of the input depth with explicit loads (also handles the
// half-resolution case, where one chain texel covers depth_scale snapshot texels).
fn load_input_depth(pixel_coordinates: vec2<i32>) -> f32 {
    let input_size = vec2<i32>(uniforms.size * uniforms.depth_scale);
    let coordinates = clamp(vec2<i32>(vec2<f32>(pixel_coordinates) * uniforms.depth_scale),
        vec2<i32>(0i), input_size - 1i);
    return textureLoad(input_depth, coordinates, 0i).r;
}

// Using 4 depths from the previous MIP, compute a weighted average for the depth of the current MIP
fn weighted_average(depth0: f32, depth1: f32, depth2: f32, depth3: f32) -> f32 {
    let depth_range_scale_factor = 0.75;
    let effect_radius = depth_range_scale_factor * 0.5 * 1.457;
    let falloff_range = 0.615 * effect_radius;
    let falloff_from = effect_radius * (1.0 - 0.615);
    let falloff_mul = -1.0 / falloff_range;
    let falloff_add = falloff_from / falloff_range + 1.0;

    let min_depth = min(min(depth0, depth1), min(depth2, depth3));
    let weight0 = saturate((depth0 - min_depth) * falloff_mul + falloff_add);
    let weight1 = saturate((depth1 - min_depth) * falloff_mul + falloff_add);
    let weight2 = saturate((depth2 - min_depth) * falloff_mul + falloff_add);
    let weight3 = saturate((depth3 - min_depth) * falloff_mul + falloff_add);
    let weight_total = weight0 + weight1 + weight2 + weight3;

    return ((weight0 * depth0) + (weight1 * depth1) + (weight2 * depth2) + (weight3 * depth3)) / weight_total;
}

// Used to share the depths from the previous MIP level between all invocations in a workgroup
var<workgroup> previous_mip_depth: array<array<f32, 8>, 8>;

@compute
@workgroup_size(8, 8, 1)
fn preprocess_depth(@builtin(global_invocation_id) global_id: vec3<u32>, @builtin(local_invocation_id) local_id: vec3<u32>) {
    let base_coordinates = vec2<i32>(global_id.xy);

    // MIP 0 - Copy 4 texels from the input depth (per invocation, 8x8 invocations per workgroup)
    let pixel_coordinates0 = base_coordinates * 2i;
    let pixel_coordinates1 = pixel_coordinates0 + vec2<i32>(1i, 0i);
    let pixel_coordinates2 = pixel_coordinates0 + vec2<i32>(0i, 1i);
    let pixel_coordinates3 = pixel_coordinates0 + vec2<i32>(1i, 1i);
    let depth0 = load_input_depth(pixel_coordinates0);
    let depth1 = load_input_depth(pixel_coordinates1);
    let depth2 = load_input_depth(pixel_coordinates2);
    let depth3 = load_input_depth(pixel_coordinates3);
    textureStore(preprocessed_depth_mip0, pixel_coordinates0, vec4<f32>(depth0, 0.0, 0.0, 0.0));
    textureStore(preprocessed_depth_mip0, pixel_coordinates1, vec4<f32>(depth1, 0.0, 0.0, 0.0));
    textureStore(preprocessed_depth_mip0, pixel_coordinates2, vec4<f32>(depth2, 0.0, 0.0, 0.0));
    textureStore(preprocessed_depth_mip0, pixel_coordinates3, vec4<f32>(depth3, 0.0, 0.0, 0.0));

    // MIP 1 - Weighted average of MIP 0's depth values (per invocation, 8x8 invocations per workgroup)
    let depth_mip1 = weighted_average(depth0, depth1, depth2, depth3);
    textureStore(preprocessed_depth_mip1, base_coordinates, vec4<f32>(depth_mip1, 0.0, 0.0, 0.0));
    previous_mip_depth[local_id.x][local_id.y] = depth_mip1;

    workgroupBarrier();

    // MIP 2 - Weighted average of MIP 1's depth values (per invocation, 4x4 invocations per workgroup)
    if all(local_id.xy % vec2<u32>(2u) == vec2<u32>(0u)) {
        let mip2_depth0 = previous_mip_depth[local_id.x + 0u][local_id.y + 0u];
        let mip2_depth1 = previous_mip_depth[local_id.x + 1u][local_id.y + 0u];
        let mip2_depth2 = previous_mip_depth[local_id.x + 0u][local_id.y + 1u];
        let mip2_depth3 = previous_mip_depth[local_id.x + 1u][local_id.y + 1u];
        let depth_mip2 = weighted_average(mip2_depth0, mip2_depth1, mip2_depth2, mip2_depth3);
        textureStore(preprocessed_depth_mip2, base_coordinates / 2i, vec4<f32>(depth_mip2, 0.0, 0.0, 0.0));
        previous_mip_depth[local_id.x][local_id.y] = depth_mip2;
    }

    workgroupBarrier();

    // MIP 3 - Weighted average of MIP 2's depth values (per invocation, 2x2 invocations per workgroup)
    if all(local_id.xy % vec2<u32>(4u) == vec2<u32>(0u)) {
        let mip3_depth0 = previous_mip_depth[local_id.x + 0u][local_id.y + 0u];
        let mip3_depth1 = previous_mip_depth[local_id.x + 2u][local_id.y + 0u];
        let mip3_depth2 = previous_mip_depth[local_id.x + 0u][local_id.y + 2u];
        let mip3_depth3 = previous_mip_depth[local_id.x + 2u][local_id.y + 2u];
        let depth_mip3 = weighted_average(mip3_depth0, mip3_depth1, mip3_depth2, mip3_depth3);
        textureStore(preprocessed_depth_mip3, base_coordinates / 4i, vec4<f32>(depth_mip3, 0.0, 0.0, 0.0));
        previous_mip_depth[local_id.x][local_id.y] = depth_mip3;
    }
}

// MIP 4: weighted average of MIP 3's depth values, as a second (tiny) dispatch.
@compute
@workgroup_size(8, 8, 1)
fn downsample_mip4(@builtin(global_invocation_id) global_id: vec3<u32>) {
    let base_coordinates = vec2<i32>(global_id.xy);
    let mip3_size = max(vec2<i32>(textureDimensions(preprocessed_depth_mip3_in)), vec2<i32>(1i));
    let coordinates0 = clamp(base_coordinates * 2i, vec2<i32>(0i), mip3_size - 1i);
    let coordinates1 = clamp(base_coordinates * 2i + vec2<i32>(1i, 0i), vec2<i32>(0i), mip3_size - 1i);
    let coordinates2 = clamp(base_coordinates * 2i + vec2<i32>(0i, 1i), vec2<i32>(0i), mip3_size - 1i);
    let coordinates3 = clamp(base_coordinates * 2i + vec2<i32>(1i, 1i), vec2<i32>(0i), mip3_size - 1i);
    let depth0 = textureLoad(preprocessed_depth_mip3_in, coordinates0, 0i).r;
    let depth1 = textureLoad(preprocessed_depth_mip3_in, coordinates1, 0i).r;
    let depth2 = textureLoad(preprocessed_depth_mip3_in, coordinates2, 0i).r;
    let depth3 = textureLoad(preprocessed_depth_mip3_in, coordinates3, 0i).r;
    let depth_mip4 = weighted_average(depth0, depth1, depth2, depth3);
    textureStore(preprocessed_depth_mip4, base_coordinates, vec4<f32>(depth_mip4, 0.0, 0.0, 0.0));
}
