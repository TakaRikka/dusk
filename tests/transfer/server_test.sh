#!/usr/bin/env bash
# End-to-end test for the transfer server: builds the host harness, uploads a file in chunks,
# proves a replayed chunk is refused, resumes from the server's reported offset, and checks the
# published result byte-for-byte.
set -euo pipefail
cd "$(dirname "$0")/../.."

cmake -S tests/transfer -B build/tests-transfer >/dev/null
cmake --build build/tests-transfer --target server_harness >/dev/null

work="$(mktemp -d)"
pid=""
cleanup() { [ -n "$pid" ] && kill "$pid" 2>/dev/null || true; rm -rf "$work"; }
trap cleanup EXIT
mkdir -p "$work/discs"

dd if=/dev/urandom of="$work/src.iso" bs=1048576 count=3 2>/dev/null
size=$(wc -c < "$work/src.iso" | tr -d ' ')

./build/tests-transfer/server_harness "$work/discs" 18080 &
pid=$!
sleep 1
base="http://127.0.0.1:18080"

id=$(curl -fsS "$base/status?name=src.iso&size=$size" | sed -n 's/.*"id":"\([^"]*\)".*/\1/p')
[ -n "$id" ] || { echo "FAIL: no id from /status"; exit 1; }

dd if="$work/src.iso" of="$work/c0" bs=1048576 count=1 2>/dev/null
curl -fsS -X POST --data-binary "@$work/c0" "$base/chunk?id=$id&offset=0" >/dev/null

# The chunk landed once. Replaying it must be refused, or the image would be silently corrupt.
code=$(curl -s -o /dev/null -w '%{http_code}' -X POST --data-binary "@$work/c0" "$base/chunk?id=$id&offset=0")
[ "$code" = "409" ] || { echo "FAIL: replayed chunk returned $code, expected 409"; exit 1; }

received=$(curl -fsS "$base/status?name=src.iso&size=$size" | sed -n 's/.*"received":\([0-9]*\).*/\1/p')
[ "$received" = "1048576" ] || { echo "FAIL: received=$received, expected 1048576"; exit 1; }

dd if="$work/src.iso" of="$work/rest" bs=1 skip="$received" 2>/dev/null
curl -fsS -X POST --data-binary "@$work/rest" "$base/chunk?id=$id&offset=$received" >/dev/null
curl -fsS -X POST "$base/finalize?id=$id" >/dev/null

published="$work/discs/src.iso"
[ -f "$published" ] || { echo "FAIL: not published to $published"; exit 1; }
cmp "$work/src.iso" "$published" || { echo "FAIL: published file differs from source"; exit 1; }
[ -z "$(ls -A "$work/discs/.incoming" 2>/dev/null)" ] || { echo "FAIL: staging not cleaned"; exit 1; }

echo "server_test OK"
