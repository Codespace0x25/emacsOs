#!/usr/bin/env bash


if grub-file --is-x86-multiboot2  $3; then
    echo multiboot confirmed
    echo running
    $1 -drive file=$2,format=raw,if=ide -serial stdio -audiodev pa,id=speaker \
       -machine pcspk-audiodev=speaker -full-screen -usb -rtc base=localtime \
       
else
    echo this file is not multiboot complient
    exit 1
fi
