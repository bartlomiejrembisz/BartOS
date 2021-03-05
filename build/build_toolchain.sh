#! /bin/bash

readonly BINUTILS_VER=2.36
readonly GCC_VER=10.2.0
readonly ROLLBACK=$(pwd)

export PREFIX="$(pwd)/toolchain/cross"
export TARGET="x86_64-elf"
export PATH="$PREFIX/bin:$PATH"

mkdir -p toolchain
cd toolchain

wget -q binutils-$BINUTILS_VER.tar.gz http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.gz
wget -q gcc-$GCC_VER.tar.gz http://mirrors.concertpass.com/gcc/releases/gcc-$GCC_VER/gcc-$GCC_VER.tar.gz
sudo apt-get -y build-essential install libgmp-dev libmpfr-dev libmpc-dev nasm cmake

alias make="make -j$(expr $(nproc) - 1)"

# Binutils
echo "Building Binutils..."

tar -zxvf binutils-$BINUTILS_VER.tar.gz
rm binutils-$BINUTILS_VER.tar.gz

mkdir -p build-binutils;
pushd build-binutils
../binutils-$BINUTILS_VER/configure --prefix=$PREFIX --target=$TARGET --with-sysroot --disable-nls --disable-werror;
make
make install
popd

# GCC
echo "Building GCC..."

tar -zxvf gcc-$GCC_VER.tar.gz
rm gcc-$GCC_VER.tar.gz

mkdir -p build-gcc
pushd build-gcc
../gcc-$GCC_VER/configure --prefix=$PREFIX --target=$TARGET --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
popd

cd $ROLLBACK
