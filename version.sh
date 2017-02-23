#!/bin/sh

SVNVERSIONCMD=`which svnversion`
if test "X$SVNVERSIONCMD" = "X" || test "`$SVNVERSIONCMD -n '.'`" = "exported" || test "`$SVNVERSIONCMD -n '.'`" = "Unversioned directory"; then
    SVN_VERSION=`cat svn_version.txt`
else
    SVN_VERSION=`svnversion -n |sed 's/\:.*$//'`
    echo ${SVN_VERSION} >svn_version.txt 
fi

VERSION=`cat VERSION`
echo -n "${VERSION}.${SVN_VERSION}"
