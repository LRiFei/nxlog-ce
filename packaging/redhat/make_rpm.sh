#!/bin/bash

NAME=nxlog
TOPDIR=`pwd`/rpmbuild
#rm -rf $TOPDIR

MKDIRLIST="$TOPDIR/BUILD/$NAME-root $TOPDIR/RPMS $TOPDIR/SOURCES $TOPDIR/SPECS $TOPDIR/SRPMS"

for dirn in $MKDIRLIST
do
	mkdir -p $dirn
done


if test x$SPEC_FILE = 'x'; then
    SPEC_FILE="$NAME.spec"
fi
RPM_SPEC="$TOPDIR/SPECS/$SPEC_FILE"

cp ../../../nxlog-*.tar.gz $TOPDIR/SOURCES/
cp $SPEC_FILE $RPM_SPEC
cd "$TOPDIR/SPECS/"
rpmbuild -bb --define="_topdir $TOPDIR" --define="_tmppath $TOPDIR/tmp" --buildroot=$TOPDIR/BUILD/$NAME-root $SPEC_FILE


#echo "Cleaning up tempfiles"
#echo "rm -r $TOPDIR"
#rm -r $TOPDIR



