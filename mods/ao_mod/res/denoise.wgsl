// 3x3 bilaterial filter (edge-preserving blur)
// https://people.csail.mit.edu/sparis/bf_course/course_notes.pdf
//
// Note: Does not use the Gaussian kernel part of a typical bilateral blur
// From the paper: "use the information gathered on a neighborhood of 4 x 4 using a bilateral filter for
// reconstruction, using _uniform_ convolution weights"
//
// Note: The paper does a 4x4 (not quite centered) filter, offset by +/- 1 pixel every other frame
// XeGTAO does a 3x3 filter, on two pixels at a time per compute thread, applied twice
// We do a 3x3 filter, on 1 pixel per compute thread, applied once
//
// Ported from Bevy Engine, crates/bevy_pbr/src/ssao/spatial_denoise.wgsl (v0.13.2), licensed
// MIT OR Apache-2.0 (see res/licenses/), itself derived from Intel XeGTAO (MIT).
//
// PORT: the textureGather calls are rewritten as explicit per-neighbor textureLoads (r32float
// and r32uint are unfilterable); Bevy view uniforms -> the mod's uniform block; r16float -> r32float.

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

@group(0) @binding(0) var ambient_occlusion_noisy: texture_2d<f32>;
@group(0) @binding(1) var depth_differences: texture_2d<u32>;
@group(0) @binding(2) var ambient_occlusion: texture_storage_2d<r32float, write>;
@group(0) @binding(3) var<uniform> uniforms: Uniforms;

fn clamp_coordinates(pixel_coordinates: vec2<i32>) -> vec2<i32> {
    return clamp(pixel_coordinates, vec2<i32>(0i), vec2<i32>(uniforms.size) - 1i);
}

// Each pixel's packed edge info is (left, right, top, bottom) weights, packed by the GTAO pass.
fn load_edges(pixel_coordinates: vec2<i32>) -> vec4<f32> {
    return unpack4x8unorm(textureLoad(depth_differences, clamp_coordinates(pixel_coordinates), 0i).r);
}

fn load_visibility(pixel_coordinates: vec2<i32>) -> f32 {
    return textureLoad(ambient_occlusion_noisy, clamp_coordinates(pixel_coordinates), 0i).r;
}

@compute
@workgroup_size(8, 8, 1)
fn spatial_denoise(@builtin(global_invocation_id) global_id: vec3<u32>) {
    let pixel_coordinates = vec2<i32>(global_id.xy);

    let left_edges = load_edges(pixel_coordinates + vec2<i32>(-1i, 0i));
    let right_edges = load_edges(pixel_coordinates + vec2<i32>(1i, 0i));
    let top_edges = load_edges(pixel_coordinates + vec2<i32>(0i, -1i));
    let bottom_edges = load_edges(pixel_coordinates + vec2<i32>(0i, 1i));
    var center_edges = load_edges(pixel_coordinates);
    // Cross-check each edge against the neighbor's opposing edge weight.
    center_edges *= vec4<f32>(left_edges.y, right_edges.x, top_edges.w, bottom_edges.z);

    let center_weight = 1.2;
    let left_weight = center_edges.x;
    let right_weight = center_edges.y;
    let top_weight = center_edges.z;
    let bottom_weight = center_edges.w;
    let top_left_weight = 0.425 * (top_weight * top_edges.x + left_weight * left_edges.z);
    let top_right_weight = 0.425 * (top_weight * top_edges.y + right_weight * right_edges.z);
    let bottom_left_weight = 0.425 * (bottom_weight * bottom_edges.x + left_weight * left_edges.w);
    let bottom_right_weight = 0.425 * (bottom_weight * bottom_edges.y + right_weight * right_edges.w);

    let center_visibility = load_visibility(pixel_coordinates);
    let left_visibility = load_visibility(pixel_coordinates + vec2<i32>(-1i, 0i));
    let right_visibility = load_visibility(pixel_coordinates + vec2<i32>(1i, 0i));
    let top_visibility = load_visibility(pixel_coordinates + vec2<i32>(0i, -1i));
    let bottom_visibility = load_visibility(pixel_coordinates + vec2<i32>(0i, 1i));
    let top_left_visibility = load_visibility(pixel_coordinates + vec2<i32>(-1i, -1i));
    let top_right_visibility = load_visibility(pixel_coordinates + vec2<i32>(1i, -1i));
    let bottom_left_visibility = load_visibility(pixel_coordinates + vec2<i32>(-1i, 1i));
    let bottom_right_visibility = load_visibility(pixel_coordinates + vec2<i32>(1i, 1i));

    // PORT: Bevy sums the center sample unweighted while still counting center_weight in the
    // denominator; XeGTAO's original weights the value too, which is what we do here.
    var sum = center_visibility * center_weight;
    sum += left_visibility * left_weight;
    sum += right_visibility * right_weight;
    sum += top_visibility * top_weight;
    sum += bottom_visibility * bottom_weight;
    sum += top_left_visibility * top_left_weight;
    sum += top_right_visibility * top_right_weight;
    sum += bottom_left_visibility * bottom_left_weight;
    sum += bottom_right_visibility * bottom_right_weight;

    var sum_weight = center_weight;
    sum_weight += left_weight;
    sum_weight += right_weight;
    sum_weight += top_weight;
    sum_weight += bottom_weight;
    sum_weight += top_left_weight;
    sum_weight += top_right_weight;
    sum_weight += bottom_left_weight;
    sum_weight += bottom_right_weight;

    let denoised_visibility = sum / sum_weight;

    textureStore(ambient_occlusion, pixel_coordinates, vec4<f32>(denoised_visibility, 0.0, 0.0, 0.0));
}
