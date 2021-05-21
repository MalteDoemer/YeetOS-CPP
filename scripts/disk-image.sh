#!/bin/bash

#SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

DISK=/dev/loop0
PART=/dev/loop1
IMAGE=$YEETOS_DISK_IMAGE
MNT=$YEETOS_USER_ROOT

if [ "$YEETOS_ARCH" != "x86" ] ; then
    echo "unsupported architecture!"
    exit
fi

dd if=/dev/zero of="$IMAGE" bs=512 count=131072
printf "o\nn\np\n1\n\n\na\np\nw\n" | fdisk "$IMAGE"


losetup -o 1048576 "$PART" "$IMAGE"
mkfs.fat -h 2048 -R 8 "$PART"
losetup -d "$PART"


losetup "$DISK" "$IMAGE"
losetup -o 1048576 "$PART" "$IMAGE"
mount "$PART" "$MNT"

grub-install --target=i386-pc --root-directory="$MNT" --no-floppy --modules="normal part_msdos fat multiboot" "$DISK"

umount "$PART"
losetup -d "$DISK"
losetup -d "$PART"