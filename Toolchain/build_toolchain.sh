#! /bin/bash

DIR=$(pwd)

ROOT=$(cd .. && pwd)

ARCH="x86_64"
PREFIX="$DIR/local/$ARCH"
TARGET="$ARCH-elf"
BUILD="$DIR/build/$ARCH"
SYSROOT="$PREFIX/root"
PATH="$PREFIX/bin:$PATH"

MAKE="make"
NPROC="nproc"

mkdir -p $BUILD

if [ -z "$MAKEJOBS" ]; then
    MAKEJOBS=$($NPROC)
fi

readonly BINUTILS_VER=2.36
readonly BINUTILS_NAME=binutils-$BINUTILS_VER
readonly BINUTILS_TAR=$BINUTILS_NAME.tar.gz
readonly BINUTILS_URL=http://ftp.gnu.org/gnu/binutils/$BINUTILS_TAR

readonly GCC_VER=10.2.0
readonly GCC_NAME=gcc-$GCC_VER
readonly GCC_TAR=$GCC_NAME.tar.gz
readonly GCC_URL=http://mirrors.concertpass.com/gcc/releases/$GCC_NAME/$GCC_TAR

readonly NEWLIB_VER=4.1.0
readonly NEWLIB_NAME=newlib-$NEWLIB_VER
readonly NEWLIB_TAR=$NEWLIB_NAME.tar.gz
readonly NEWLIB_URL=ftp://sourceware.org/pub/newlib/$NEWLIB_TAR

echo "Fetching dependencies (sudo apt-get)..."
wget -Nq $BINUTILS_TAR $BINUTILS_URL &
wget -Nq $GCC_TAR $GCC_URL &
wget -Nq $NEWLIB_TAR $NEWLIB_URL &
sudo apt-get install -y build-essential libgmp-dev libmpfr-dev libmpc-dev nasm cmake &
wait

if [[ ! -d "$BINUTILS_NAME" ]]; then
    tar -zxvf $BINUTILS_TAR &
fi

if [[ ! -d "$NEWLIB_NAME" ]]; then
	tar -zxvf $NEWLIB_TAR &
fi

if [[ ! -d "$GCC_NAME" ]]; then
	tar -zxvf $GCC_TAR
		
	# Patching GCC
	pushd $GCC_NAME
		# GCC deps
		./contrib/download_prerequisites
		
		# Link bthread wrapper header.
		rm -f "$DIR"/"$GCC_NAME"/libgcc/gthr-bthread.h
		cp "$ROOT"/Kernel/Thread/gthr-bthread.h "$DIR"/"$GCC_NAME"/libgcc/gthr-bthread.h

		# Patch gcc with bthread
		patch -p1 < "$DIR"/Patch/bthread-support.patch > /dev/null
	popd
fi

wait

# Binutils
echo "Building $BINUTILS_NAME..."
mkdir -p "$BUILD/$BINUTILS_NAME"
pushd "$BUILD/$BINUTILS_NAME"
	"$DIR"/$BINUTILS_NAME/configure --prefix="$PREFIX" --target="$TARGET" --with-sysroot="$SYSROOT" --enable-shared --disable-nls --disable-werror
	"$MAKE" -j "$MAKEJOBS" || exit 1
	"$MAKE" install || exit 1
popd

# GCC 1
echo "Building $GCC_NAME..."
mkdir -p "$BUILD/$GCC_NAME"
pushd "$BUILD/$GCC_NAME"
	"$DIR"/$GCC_NAME/configure 	--prefix="$PREFIX"		\
					--target="$TARGET"		\
					--with-sysroot="$SYSROOT"	\
					--enable-languages=c		\
					--disable-nls			\
					--without-headers		\
					--with-newlib			\
					--disable-shared		\
					--disable-multilib		\
					--enable-threads=bthread
					
	C_FLAGS='-g -O2 -mcmodel=large -mno-red-zone' CXX_FLAGS='-g -O2 -mcmodel=large -mno-red-zone' "$MAKE" -j "$MAKEJOBS" all-gcc || exit 1
	"$MAKE" install-gcc || exit 1
popd

# Newlib
echo "Building $NEWLIB_NAME..."
mkdir -p "$BUILD/$NEWLIB_NAME"
pushd "$BUILD/$NEWLIB_NAME"
	"$DIR"/$NEWLIB_NAME/configure --prefix=$PREFIX --target="$TARGET" --disable-multilib --disable-newlib-multithread
	C_FLAGS='-g -O2 -mcmodel=large -mno-red-zone' "$MAKE" -j "$MAKEJOBS" || exit 1
	"$MAKE" install || exit 1
popd

# GCC 2
echo "Building $GCC_NAME-2..."
mkdir -p "$BUILD/$GCC_NAME-2"
pushd "$BUILD/$GCC_NAME-2"
	"$DIR"/$GCC_NAME/configure 	--prefix="$PREFIX"					\
					--target="$TARGET"					\
					--with-sysroot="$SYSROOT"				\
					--enable-languages=c,c++				\
					--disable-nls 						\
					--with-newlib						\
					--disable-shared					\
					--disable-multilib					\
					--enable-threads=bthread				\
					--enable-libstdcxx-threads				\
					--enable-libstdcxx-debug				\
					--enable-cxx-flags='-fno-exceptions -fno-rtti -mcmodel=large -mno-red-zone'	\
					--disable-libstdcxx-filesystem-ts
					#--enable-libstdcxx-allocator=		\
	
	C_FLAGS='-g -O2 -mcmodel=large -mno-red-zone' CXX_FLAGS='-g -O2 -mcmodel=large -mno-red-zone' "$MAKE" -j "$MAKEJOBS" || exit 1
	"$MAKE" install || exit 1
	
	#mkdir -p "$SYSROOT/usr/include/"
	#for header in ../../$GCC_NAME/gcc/ginclude/*.h; do
	#	install -D "$header" "$SYSROOT/usr/include"
	#done
	
	#"$MAKE" -j "$MAKEJOBS" all-target-libstdc++-v3 || exit 1
	#"$MAKE" install-target-libstdc++-v3 || exit 1
popd	

cd $DIR
