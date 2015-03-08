#!/bin/sh
# Usage: copyshared.sh <binary> <dest folder>
# Copies all shared libraries needed by a binary to a folder.
# Note: This only works on Linux.

BIN=`echo $1 | sed 's/ /\\\\ /g'`
DST=`echo $2 | sed 's/ /\\\\ /g'`
eval mkdir -p ${DST}
eval ldd ${BIN} | \
    grep "=>" | \
    sed "s/.*\s*=>\s*\(.*\)(.*)/\1/" | \
    sed "s/^[ ]//g" | \
    sed "s/[ ]$//g" | \
    eval xargs -I{} cp -L {} ${DST}
