"""
Loads d_a_mant.rel from CWD, applies tears using seed (66, 16983, 855) and cut type 1, writes PNGs to tear_textures/
Requires pillow and xxhash.
"""

import math
import struct
import xxhash
from pathlib import Path
from PIL import Image

def yaz0_decompress(data):
    expand_size = struct.unpack_from(">I", data, 4)[0]
    out = bytearray(expand_size)
    src_pos = 0x10
    dst_pos = 0
    chunk_bits_left = 0
    chunk_bits = 0

    while dst_pos < expand_size:
        if chunk_bits_left == 0:
            chunk_bits = data[src_pos]
            src_pos += 1
            chunk_bits_left = 8

        if chunk_bits & 0x80:
            out[dst_pos] = data[src_pos]
            src_pos += 1
            dst_pos += 1
        else:
            b0 = data[src_pos]
            b1 = data[src_pos + 1]
            src_pos += 2
            dist = ((b0 & 0x0F) << 8) | b1
            count = b0 >> 4
            if count == 0:
                count = data[src_pos] + 0x12
                src_pos += 1
            else:
                count += 2

            copy_pos = dst_pos - dist - 1
            for _ in range(count):
                out[dst_pos] = out[copy_pos]
                dst_pos += 1
                copy_pos += 1

        chunk_bits <<= 1
        chunk_bits_left -= 1

    return bytes(out)

SINCOS_TABLE = tuple(
    (
        math.sin((i * math.tau) / (1 << 13)),
        math.cos((i * math.tau) / (1 << 13)),
    )
    for i in range(1 << 13)
)

def rnd(rng):
    rng[0] = (rng[0] * 171) % 30269
    rng[1] = (rng[1] * 172) % 30307
    rng[2] = (rng[2] * 170) % 30323
    value = rng[0] / 30269.0 + rng[1] / 30307.0 + rng[2] / 30323.0
    return abs(value % 1.0)

def rnd_f(rng, max_value):
    return rnd(rng) * max_value

def rnd_fx(rng, max_value):
    return max_value * (rnd(rng) - 0.5) * 2.0

def linear_index_to_swizzled(linear_index):
    within_tile_x = linear_index & 0x7
    tile_row_offset = (linear_index & 0x78) * 4
    tile_column_offset = (linear_index >> 4) & 0x18
    macro_row_offset = linear_index & 0x3E00

    return within_tile_x + tile_row_offset + tile_column_offset + macro_row_offset

SWIZZLED_TO_XY = [(0, 0)] * 0x4000
for linear_index in range(0x4000):
    x = linear_index & 0x7F
    y = linear_index >> 7
    swizzled_index = linear_index_to_swizzled(linear_index)
    SWIZZLED_TO_XY[swizzled_index] = (x, y)

NEIGHBOR_OFFSETS = (0, 1, 0x80, 0x81, 2, 0x82, 0x102, 0x101, 0x100)

def write_c8_texture(c8_data, stage, output_dir, palette, palette_data):
    min_index = min(c8_data)
    max_index = max(c8_data)
    tlut_offset = 2 * min_index
    tlut_size = 2 * (max_index + 1 - min_index)
    tlut_end = tlut_offset + tlut_size
    path = output_dir / (
        f"[{stage:02d}] tex1_{0x80}x{0x80}_"
        f"{xxhash.xxh64(c8_data, seed=0).intdigest():016x}_"
        f"{xxhash.xxh64(palette_data[tlut_offset:tlut_end], seed=0).intdigest():016x}_"
        f"{0x9}.png"
    )

    rgba = Image.new("RGBA", (0x80, 0x80))
    pixels = rgba.load()
    for swizzled_index, palette_index in enumerate(c8_data):
        x, y = SWIZZLED_TO_XY[swizzled_index]
        pixels[x, y] = palette[palette_index]
    rgba.save(path)
    return path

def write_stage(tex, tex_u, pal, stage, output_dir, palette):
    return [
        write_c8_texture(bytes(tex), stage, output_dir, palette, pal),
        write_c8_texture(bytes(tex_u), stage, output_dir, palette, pal),
    ]

def export_mant_tears():
    rel = yaz0_decompress(Path("d_a_mant.rel").read_bytes())
    tex = bytearray(rel[0x1C00 : 0x1C00 + 0x4000])
    tex_u = bytearray([6] * 0x4000)
    pal = bytes(rel[0x9C00 : 0x9C00 + 0x60])
    rng = [int(66), int(16983), int(855)]

    Path("tear_textures").mkdir(parents=True, exist_ok=True)
    written = []

    rgba_palette = []
    for offset in range(0, len(pal), 2):
        color16 = struct.unpack_from(">H", pal, offset)[0]
        if color16 & 0x8000:
            r = (color16 >> 7) & 0xF8
            r |= r >> 5
            g = (color16 >> 2) & 0xF8
            g |= g >> 5
            b = (color16 << 3) & 0xF8
            b |= b >> 5
            a = 255
        else:
            r = (color16 >> 4) & 0xF0
            r |= r >> 4
            g = color16 & 0xF0
            g |= g >> 4
            b = (color16 << 4) & 0xF0
            b |= b >> 4
            a = (color16 >> 7) & 0xE0
            a |= (a >> 3) | (a >> 6)
        rgba_palette.append((r, g, b, a))
    while len(rgba_palette) < 256:
        rgba_palette.append((0, 0, 0, 0))

    written.extend(write_stage(tex, tex_u, pal, 0, Path("tear_textures"), rgba_palette))

    cut_step = 0
    for _ in range(15):
        cut_step += 1

        angle = int(rnd_f(rng, 65536.0)) & 0xFFFF
        if angle >= 0x8000:
            angle -= 0x10000

        x = rnd_fx(rng, 32.0)
        y = rnd_fx(rng, 32.0)
        sincos_index = (angle & 0xFFFF) >> (16 - 13)
        sin_v, cos_v = SINCOS_TABLE[sincos_index]

        for i, texel_count in enumerate(
            tuple(
                1 if i <= 3 or i >= 26 else (9 if 12 <= i <= 18 else 4)
                for i in range(30)
            )
        ):
            x += sin_v
            y -= cos_v

            packed = int(x + 64.0) | (int(y + 64.0) << 7)
            for j in range(texel_count):
                u_var1 = packed + NEIGHBOR_OFFSETS[j]
                if 0 <= u_var1 < 0x4000:
                    i_var5 = linear_index_to_swizzled(u_var1)
                    tex[i_var5] = 0
                    tex_u[i_var5] = 0

        written.extend(write_stage(tex, tex_u, pal, cut_step, Path("tear_textures"), rgba_palette))

    return written

if __name__ == "__main__":
    export_mant_tears()
