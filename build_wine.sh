set -xe

if [ -n "$CLEAN_BUILD" ]
then
	rm -rf wine_build
	rm -rf wine_build_32
fi

if ! [ -e wine_build ]
then
	CONFIGURE=true
fi

mkdir -p wine_build/install
cd wine_build
if [ "$CONFIGURE" == true ]
then
	../configure --enable-win64 --prefix=$(pwd)/install
fi
make -j$(nproc)
make install

cd ..

if ! [ -e wine_build_32 ]
then
	CONFIGURE=true
fi

mkdir -p wine_build_32/install
cd wine_build_32
if [ "$CONFIGURE" == true ]
then
	../configure --prefix=$(pwd)/install
fi
make -j$(nproc)
make install
