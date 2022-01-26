#!/bin/bash

# environement variables defined in cmake
ARCH=$YEETOS_ARCH
KERNEL=$YEETOS_KERNEL
GRUB_CFG=$YEETOS_GRUB_CFG
ISO_FILE="$OUT_DIR/yeetos.iso"

# TODO: dynamicly locate these
GRUB_FILES="/usr/lib/grub/i386-pc"

trap cleanup EXIT

TEMP_DIR=`mktemp -d`

if [[ ! "$TEMP_DIR" || ! -d "$TEMP_DIR" ]]; then
    echo "failed to create temp directory"
    exit 1
fi

mkdir -p "$TEMP_DIR/boot/grub"
cp "$KERNEL" "$TEMP_DIR/boot/YeetOS"
cp "$GRUB_CFG" "$TEMP_DIR/boot/grub/grub.cfg"  

grub-mkrescue -o "$ISO_FILE" "$TEMP_DIR" -d "$GRUB_FILES" -quiet

function cleanup {
    rm -rf "$TEMP_DIR"
}

