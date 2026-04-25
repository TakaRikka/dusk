"""
Convert a folder of TPGZ saves to a states.json

Usage:
    python saves_to_states_json.py path/to/saves [prefix]

Requirements:
    pip install zstandard
"""

import base64
import json
import struct
import sys
import zstandard
from pathlib import Path

SAVE_C_SIZE   = 0x958

PACKET_FORMAT = "<8sbbh"

RETURN_PLACE_OFF  = 0x058
NAME_OFF          = RETURN_PLACE_OFF + 0x00
ROOM_OFF          = RETURN_PLACE_OFF + 0x09
SPAWN_POINT_OFF   = RETURN_PLACE_OFF + 0x08

folder = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).parent
out_path = folder / "states.json"

if len(sys.argv) > 2:
    prefix = sys.argv[2]
else:
    prefix = None

cctx = zstandard.ZstdCompressor(level=1)
states = []

for bin_path in sorted(folder.glob("*.bin")):
    raw = bin_path.read_bytes()
    save_c = raw[:SAVE_C_SIZE]
    if len(save_c) < SAVE_C_SIZE:
        print(f"  skip {bin_path.name}: too small ({len(save_c)} bytes)")
        continue

    stage_name  = save_c[NAME_OFF:NAME_OFF + 8]
    room_no     = struct.unpack_from("b", save_c, ROOM_OFF)[0]
    spawn_point = struct.unpack_from("B", save_c, SPAWN_POINT_OFF)[0]

    pkt     = struct.pack(PACKET_FORMAT, stage_name, room_no, -1, spawn_point)
    payload = pkt + save_c
    encoded = base64.b64encode(cctx.compress(payload)).decode("ascii")

    stage_str = stage_name.rstrip(b"\x00").decode("ascii", errors="replace")
    print(f"  {bin_path.stem:30s}  stage={stage_str!r}  room={room_no}  point={spawn_point}")
    states.append({"name": f"({prefix}) {bin_path.stem}" if prefix else bin_path.stem, "data": encoded})

out_path.write_text(json.dumps(states, indent=2))
print(f"\nWrote {len(states)} states to {out_path}")
