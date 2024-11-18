#!/usr/bin/env sh

config=$1
if [ $config == "" ]; then
    config=Debug
fi

qemu-system-i386 -hda $(dirname $0)/../out/Clang/x86/$config/funny.img -serial stdio > qemu.log
