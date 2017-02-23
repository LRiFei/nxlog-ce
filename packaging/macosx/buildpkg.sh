#!/bin/bash
DSTLOCATION=/Applications/nxlog.app
CURDIR=`pwd`
INSTDIR=`pwd`/nxlog.app
rm -rf $INSTDIR
mkdir $INSTDIR
cd ../..
CFLAGS=-I/opt/local/include ./configure --enable-xm_perl=no \
 --prefix=$DSTLOCATION \
 --libdir=$DSTLOCATION/lib \
 --with-moduledir=$DSTLOCATION/modules \
 --with-configfile=$DSTLOCATION/conf/nxlog.conf \
 --with-pidfile=$DSTLOCATION/tmp/nxlog.pid \
 --with-cachedir=$DSTLOCATION/tmp \
 || exit 1
make DESTDIR=$INSTDIR install

mv $INSTDIR$DSTLOCATION/bin $INSTDIR/
mkdir $INSTDIR/conf
cp $CURDIR/nxlog.conf $INSTDIR/conf/
mkdir $INSTDIR/lib
mv $INSTDIR$DSTLOCATION/lib/libnx.0.dylib $INSTDIR/lib
mkdir $INSTDIR/modules
mkdir $INSTDIR/modules/extension
mkdir $INSTDIR/modules/input
mkdir $INSTDIR/modules/output
mkdir $INSTDIR/modules/processor
mv $INSTDIR$DSTLOCATION/modules/extension/*.so $INSTDIR/modules/extension
mv $INSTDIR$DSTLOCATION/modules/input/*.so $INSTDIR/modules/input
mv $INSTDIR$DSTLOCATION/modules/output/*.so $INSTDIR/modules/output
mv $INSTDIR$DSTLOCATION/modules/processor/*.so $INSTDIR/modules/processor
mkdir $INSTDIR/doc
mv $INSTDIR$DSTLOCATION/share/doc/nxlog/* $INSTDIR/doc
mkdir $INSTDIR/tmp

rm -rf $INSTDIR/Applications

cd $INSTDIR/..
pkgbuild --root nxlog.app --identifier com.nxsec.nxlog --install-location /Applications/nxlog.app nxlog.pkg
