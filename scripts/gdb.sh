#!/bin/bash

# environement variables defined in cmake
KERNEL=$YEETOS_KERNEL

gdb -ex "target remote localhost:9000" -ex "symbol-file $KERNEL" -ex "set disassembly-flavor intel"