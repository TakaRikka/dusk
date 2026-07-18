// Deferred shadow composite: reconstructs the world position of every scene pixel from the
// depth snapshot (CameraService matrices), transforms it into the light's clip space, and
// PCF-compares against the shadow map rendered earlier this frame. Drawn as a fullscreen
// triangle with multiply blending (srcFactor = Dst, dstFactor = Zero) before the HUD.
//
// Depth conventions (both reversed-Z): the scene snapshot has 1.0 at the camera near plane;
// the shadow map, rendered through the game's GX pipeline with a GC-convention light matrix,
// stores clip.z, i.e. 1.0 nearest to the light and 0.0 at the light frustum far plane.
//
// The optional contact-shadow raymarch follows Panos Karabelas' screen-space shadows
// (https://panoskarabelas.com/blog/posts/screen_space_shadows/, MIT via Spartan Engine):
// march from the pixel toward the light in view space and mark occlusion when the ray dips
// behind the depth buffer within a thickness threshold.

struct Uniforms {
    world_from_proj: mat4x4f, // scene depth unproject (camera)
    view_from_proj: mat4x4f,  // scene depth -> view space (contact shadows)
    proj_from_view: mat4x4f,  // view -> clip (contact shadows re-projection)
    light_vp: mat4x4f,        // world -> light receiver projection (UV/depth basis)
    light_dir_view: vec3f,    // direction *toward* the light, view space, normalized
    bias: f32,                // shadow-map depth bias (reversed-depth units)
    size: vec2f,              // shadow map size in texels
    inv_size: vec2f,
    edge_fade_width: f32,
    strength: f32,            // final darkening amount, horizon fade baked in
    pcf_taps: f32,            // 0 = single tap, 1 = 3x3, 2 = 5x5
    contact_enabled: f32,
    contact_thickness: f32,   // view-space thickness threshold
    contact_length: f32,      // view-space march distance
    debug_mode: u32,          // 0 = composite; nonzero modes are diagnostic views
    _pad0: f32,
}

@group(0) @binding(0) var scene_depth: texture_2d<f32>;
@group(0) @binding(1) var shadow_map: texture_2d<f32>;
@group(0) @binding(2) var<uniform> uniforms: Uniforms;
@group(0) @binding(3) var light_color: texture_2d<f32>;

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) uv: vec2f,
}

@vertex
fn vs_main(@builtin(vertex_index) index: u32) -> VertexOutput {
    var out: VertexOutput;
    let uv = vec2f(f32((index << 1u) & 2u), f32(index & 2u));
    out.position = vec4f(uv * vec2f(2.0, -2.0) + vec2f(-1.0, 1.0), 0.0, 1.0);
    out.uv = uv;
    return out;
}

fn load_shadow(texel: vec2<i32>) -> f32 {
    let clamped = clamp(texel, vec2<i32>(0i), vec2<i32>(uniforms.size) - 1i);
    return textureLoad(shadow_map, clamped, 0i).r;
}

// Returns 1.0 when the pixel at light-space depth `receiver` is shadowed by the map texel.
fn shadow_test(texel: vec2<i32>, receiver: f32) -> f32 {
    // Reversed depth: a larger stored value is closer to the light, i.e. an occluder.
    return select(0.0, 1.0, load_shadow(texel) > receiver + uniforms.bias);
}

// Bilinearly weighted comparison (what a hardware comparison sampler would do): filter the
// four *comparison results*, never the depths themselves. This is what turns per-texel
// staircases into smooth penumbra edges.
fn shadow_compare_bilinear(light_uv: vec2f, receiver: f32) -> f32 {
    let coordinates = light_uv * uniforms.size - 0.5;
    let base = floor(coordinates);
    let fraction = coordinates - base;
    let texel = vec2<i32>(base);
    let s00 = shadow_test(texel, receiver);
    let s10 = shadow_test(texel + vec2<i32>(1i, 0i), receiver);
    let s01 = shadow_test(texel + vec2<i32>(0i, 1i), receiver);
    let s11 = shadow_test(texel + vec2<i32>(1i, 1i), receiver);
    let top = mix(s00, s10, fraction.x);
    let bottom = mix(s01, s11, fraction.x);
    return mix(top, bottom, fraction.y);
}

fn sample_shadow_pcf(light_uv: vec2f, receiver: f32) -> f32 {
    let radius = i32(uniforms.pcf_taps);
    var sum = 0.0;
    var count = 0.0;
    for (var y = -radius; y <= radius; y += 1i) {
        for (var x = -radius; x <= radius; x += 1i) {
            let offset = vec2f(f32(x), f32(y)) * uniforms.inv_size;
            sum += shadow_compare_bilinear(light_uv + offset, receiver);
            count += 1.0;
        }
    }
    return sum / count;
}

// Softly fades shadows out over a small band near the shadow-map edge so receivers do not
// disappear abruptly when they leave the light's coverage area.
fn shadow_edge_fade(light_uv: vec2f) -> f32 {
    let edge_texels = uniforms.edge_fade_width;
    let edge_uv = edge_texels * max(uniforms.inv_size.x, uniforms.inv_size.y);
    let distance_to_edge = min(min(light_uv.x, 1.0 - light_uv.x), min(light_uv.y, 1.0 - light_uv.y));
    // Avoid division by zero when the fade width is zero (no fade).
    return select(1.0, saturate(distance_to_edge / edge_uv), edge_uv > 0.0);
}

fn scene_depth_at(uv: vec2f) -> f32 {
    let size = vec2<i32>(textureDimensions(scene_depth));
    let texel = clamp(vec2<i32>(uv * vec2f(size)), vec2<i32>(0i), size - 1i);
    return textureLoad(scene_depth, texel, 0i).r;
}

fn light_color_at(uv: vec2f) -> vec4f {
    let size = vec2<i32>(textureDimensions(light_color));
    let texel = clamp(vec2<i32>(uv * vec2f(size)), vec2<i32>(0i), size - 1i);
    return textureLoad(light_color, texel, 0i);
}

fn light_depth_debug_at(uv: vec2f) -> vec3f {
    let texel = vec2<i32>(uv * uniforms.size);
    let depth = load_shadow(texel);
    if depth <= 0.0 {
        return vec3f(0.0);
    }

    let dx = abs(depth - load_shadow(texel + vec2<i32>(1i, 0i)));
    let dy = abs(depth - load_shadow(texel + vec2<i32>(0i, 1i)));
    let edge = saturate((dx + dy) * 500.0);
    let shade = saturate(depth * 1.5);
    let bands = 0.08 * (0.5 + 0.5 * cos(depth * 96.0));
    return vec3f(saturate(shade + bands + edge));
}

fn view_position(uv: vec2f, depth: f32) -> vec3f {
    let ndc = vec4f(uv.x * 2.0 - 1.0, 1.0 - 2.0 * uv.y, depth, 1.0);
    let position = uniforms.view_from_proj * ndc;
    return position.xyz / position.w;
}

// Interleaved gradient noise (Jimenez); fixed per-pixel dither, no temporal rotation.
fn ign(pixel: vec2f) -> f32 {
    return fract(52.9829189 * fract(dot(pixel, vec2f(0.06711056, 0.00583715))));
}

// Screen-space contact shadows: march toward the light in view space; occluded when the ray
// passes behind the depth buffer by less than the thickness threshold. Faded out with view
// distance: position reconstruction error grows with distance while the thickness threshold
// is fixed, so far surfaces (and anything translucent composited over them - clouds, fog)
// would otherwise pick up dithered false occlusion. Contact shadows are a near-field effect.
fn contact_shadow_fade(view_distance: f32) -> f32 {
    return saturate(1.0 - (view_distance - 3000.0) / 5000.0);
}

fn contact_shadow(origin: vec3f, pixel: vec2f) -> f32 {
    let steps = 24;
    let step_vec = uniforms.light_dir_view * (uniforms.contact_length / f32(steps));
    var ray = origin + step_vec * ign(pixel);
    for (var i = 0; i < steps; i += 1) {
        ray += step_vec;
        // Project the ray position back to screen space.
        let clip = uniforms.proj_from_view * vec4f(ray, 1.0);
        if clip.w <= 0.0 {
            break;
        }
        let ray_ndc = clip.xyz / clip.w;
        let ray_uv = vec2f(0.5 + 0.5 * ray_ndc.x, 0.5 - 0.5 * ray_ndc.y);
        if any(ray_uv < vec2f(0.0)) || any(ray_uv > vec2f(1.0)) {
            break;
        }
        let scene = scene_depth_at(ray_uv);
        if scene <= 0.0 {
            continue;
        }
        // Compare in view space: positive delta = the ray is behind the scene surface.
        let scene_z = view_position(ray_uv, scene).z;
        let delta = scene_z - ray.z; // view space looks down -z; larger z = closer
        if delta > 0.0 && delta < uniforms.contact_thickness {
            return 1.0;
        }
    }
    return 0.0;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    let depth = scene_depth_at(in.uv);
    if uniforms.debug_mode == 1u {
        let value = load_shadow(vec2<i32>(in.uv * uniforms.size));
        return vec4f(value, value, value, 1.0);
    }
    if uniforms.debug_mode == 9u || uniforms.debug_mode == 10u {
        let color = light_color_at(in.uv);
        let color_luma = max(color.r, max(color.g, color.b));
        let depth_color = light_depth_debug_at(in.uv);
        let rgb = select(depth_color, color.rgb, color_luma > (1.0 / 255.0));
        return vec4f(rgb, 1.0);
    }

    if depth <= 0.0 {
        // Sky / cleared pixels receive no shadow.
        if uniforms.debug_mode >= 3u {
            return vec4f(0.0, 0.0, 0.0, 1.0);
        }
        return vec4f(1.0);
    }

    let ndc = vec4f(in.uv.x * 2.0 - 1.0, 1.0 - 2.0 * in.uv.y, depth, 1.0);
    let world4 = uniforms.world_from_proj * ndc;
    let world = world4.xyz / world4.w;

    let light_clip = uniforms.light_vp * vec4f(world, 1.0);
    let light_ndc = light_clip.xyz / light_clip.w;
    let receiver = light_ndc.z; // reversed light depth, 1 = nearest to the light
    let light_uv = vec2f(0.5 + 0.5 * light_ndc.x, 0.5 - 0.5 * light_ndc.y);
    let in_shadow_bounds = all(light_uv >= vec2f(0.0)) && all(light_uv <= vec2f(1.0)) &&
        receiver > 0.0 && receiver <= 1.0;
    let shadow_depth = load_shadow(vec2<i32>(light_uv * uniforms.size));

    if uniforms.debug_mode == 4u {
        let valid = select(0.0, 1.0, in_shadow_bounds);
        return vec4f(saturate(light_uv.x), saturate(light_uv.y), valid, 1.0);
    }

    if uniforms.debug_mode == 5u {
        if !in_shadow_bounds {
            return vec4f(0.0, 0.0, 0.0, 1.0);
        }
        let current_compare = select(0.0, 1.0, shadow_depth > receiver + uniforms.bias);
        let opposite_compare = select(0.0, 1.0, shadow_depth < receiver - uniforms.bias);
        return vec4f(current_compare, 0.0, opposite_compare, 1.0);
    }

    if uniforms.debug_mode == 6u {
        let valid = select(0.0, 1.0, in_shadow_bounds);
        return vec4f(saturate(receiver), shadow_depth, valid, 1.0);
    }

    if uniforms.debug_mode == 7u {
        let beyond_far = select(0.0, 1.0, receiver <= 0.0);
        let valid_depth = select(0.0, 1.0, receiver > 0.0 && receiver <= 1.0);
        let before_near = select(0.0, 1.0, receiver > 1.0);
        return vec4f(beyond_far, valid_depth, before_near, 1.0);
    }

    if uniforms.debug_mode == 8u {
        let valid_x = select(0.0, 1.0, light_uv.x >= 0.0 && light_uv.x <= 1.0);
        let valid_y = select(0.0, 1.0, light_uv.y >= 0.0 && light_uv.y <= 1.0);
        let valid_depth = select(0.0, 1.0, receiver > 0.0 && receiver <= 1.0);
        return vec4f(valid_x, valid_y, valid_depth, 1.0);
    }

    var occlusion = 0.0;
    if in_shadow_bounds {
        occlusion = sample_shadow_pcf(light_uv, receiver);
        occlusion *= shadow_edge_fade(light_uv);
    }

    if uniforms.debug_mode == 3u {
        return vec4f(occlusion, occlusion, occlusion, 1.0);
    }

    if uniforms.contact_enabled != 0.0 && occlusion < 1.0 {
        let origin = view_position(in.uv, depth);
        let fade = contact_shadow_fade(-origin.z);
        if fade > 0.0 {
            occlusion = max(occlusion, fade * contact_shadow(origin, in.position.xy));
        }
    }

    let value = 1.0 - uniforms.strength * occlusion;
    if uniforms.debug_mode == 2u {
        return vec4f(value, value, value, 1.0);
    }
    return vec4f(value, value, value, 1.0);
}
