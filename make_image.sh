#!/usr/bin/env bash
set -euo pipefail

boot="$1"
kernel="$2"
image="$3"

test "$(stat -c %s "$boot")" -eq 512
test "$(stat -c %s "$kernel")" -le 16384

truncate -s 1440K "$image"
dd if="$boot" of="$image" conv=notrunc status=none
dd if="$kernel" of="$image" bs=512 seek=1 conv=notrunc status=none
