rm -rf /c/msys/1.0/nxlog-tmp
cd ../..
./configure --with-moduledir="/" --prefix="/" --bindir="/" --libdir="/" --with-cachedir="/" || exit 1
make DESTDIR='C:\\\\msys\\\\1.0\\\\nxlog-tmp' install || exit 1

for i in libnx-0.dll libiconv-2.dll libapr-1-0.dll libpcre-0.dll cryptoeay32-0.9.8.dll ssleay32-0.9.8.dll libexpat-1.dll zlib1.dll; do
    cp /c/msys/1.0/bin/$i /c/msys/1.0/nxlog-tmp/
done

cp packaging/windows/* /c/msys/1.0/nxlog-tmp/
strip /c/msys/1.0/nxlog-tmp/extension/*.dll
rm -f /c/msys/1.0/nxlog-tmp/extension/*.a
rm -f /c/msys/1.0/nxlog-tmp/extension/*.la
strip /c/msys/1.0/nxlog-tmp/input/*.dll
rm -f /c/msys/1.0/nxlog-tmp/input/*.a
rm -f /c/msys/1.0/nxlog-tmp/input/*.la
strip /c/msys/1.0/nxlog-tmp/processor/*.dll
rm -f /c/msys/1.0/nxlog-tmp/processor/*.a
rm -f /c/msys/1.0/nxlog-tmp/processor/*.la
strip /c/msys/1.0/nxlog-tmp/output/*.dll
rm -f /c/msys/1.0/nxlog-tmp/output/*.a
rm -f /c/msys/1.0/nxlog-tmp/output/*.la
rm -f /c/msys/1.0/nxlog-tmp/*.la
rm -f /c/msys/1.0/nxlog-tmp/*.a
strip /c/msys/1.0/nxlog-tmp/*.dll
strip /c/msys/1.0/nxlog-tmp/*.exe
mkdir /c/msys/1.0/nxlog-tmp/modules
mv /c/msys/1.0/nxlog-tmp/extension /c/msys/1.0/nxlog-tmp/modules/
mv /c/msys/1.0/nxlog-tmp/input /c/msys/1.0/nxlog-tmp/modules/
mv /c/msys/1.0/nxlog-tmp/processor /c/msys/1.0/nxlog-tmp/modules/
mv /c/msys/1.0/nxlog-tmp/output /c/msys/1.0/nxlog-tmp/modules/
mv /c/msys/1.0/nxlog-tmp/share/doc/nxlog-ce /c/msys/1.0/nxlog-tmp/doc
rm -rf /c/msys/1.0/nxlog-tmp/share
