#!/bin/bash

IMAGE=$YEETOS_DISK_IMAGE

qemu-system-i386 -drive format=raw,file=$IMAGE,if=ide -S -gdb tcp::9000 -m 512 -name "YeetOS" -serial stdio -k ch
