#!/bin/bash

# environement variables defined in cmake
ARCH=$YEETOS_ARCH
CONFIG=$YEETOS_CONFIG
ISO_FILE="$OUT_DIR/yeetos.iso"


if [ "$ARCH" == "x86" ] ; then
    qemu-system-i386 -S -gdb tcp::9000 -m 2G -name "YeetOS" -serial stdio -cdrom "$ISO_FILE"
else
    echo "unsupported architecture: $ARCH"
    exit
fi
