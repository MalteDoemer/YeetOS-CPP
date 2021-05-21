#!/bin/bash

IMAGE=$YEETOS_DISK_IMAGE
HDD=/dev/loop0
PART=/dev/loop1
MNT=$YEETOS_USER_ROOT

losetup "$HDD" "$IMAGE"
losetup -o 1048576 "$PART" "$IMAGE"
mount "$PART" "$MNT"

mkdir -p "$MNT/boot"
mkdir -p "$MNT/boot/grub"
cp "$YEETOS_GRUB_CFG" "$MNT/boot/grub/grub.cfg"
cp "$YEETOS_KERNEL" "$MNT/boot/YeetOS"

umount "$PART"
losetup -d "$HDD"
losetup -d "$PART"