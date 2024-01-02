#!/bin/bash
#设置环境

export PATH=$PATH:$(pwd)/../Compiler/ZyC_Clang_18/bin
export CC=clang
export CLANG_TRIPLE=aarch64-linux-gnu-
export CROSS_COMPILE=aarch64-linux-gnu-
export CROSS_COMPILE_ARM32=arm-linux-gnueabi-
export CONFIG_BUILD_ARM64_DT_OVERLAY=y

export ARCH=arm64
export DTC_EXT=dtc

make ARCH=arm64 O=out CC=clang ruby_stock_mod_ksu_defconfig
make ARCH=arm64 O=out CC=clang menuconfig