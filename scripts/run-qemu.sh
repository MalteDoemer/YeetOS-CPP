#!/bin/bash

IMAGE=$YEETOS_DISK_IMAGE


if [ "$YEETOS_ARCH" != "x86" ] ; then
    echo "unsupported architecture!"
    exit
fi


qemu-system-i386 -accel kvm -drive format=raw,file="$IMAGE",if=ide -m 512 -name "YeetOS" -serial stdio -k ch
