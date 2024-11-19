#!/usr/bin/env sh

config=$1
if [ $# -lt 2 ]; then
    config=Debug
fi

qemu-system-i386 -hda $(dirname $0)/../out/Clang/x86/$config/funny.img -serial stdio > qemu.log
