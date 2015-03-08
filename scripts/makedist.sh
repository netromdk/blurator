#!/bin/sh
# Usage: makedist.sh <binary dir> <dist dir> <zip dist>

BIN_DIR=`echo $1 | sed 's/ /\\\\ /g'`
DIST_DIR=`echo $2 | sed 's/ /\\\\ /g'`
DIST_BASE=`eval basename ${DIST_DIR}`
DIST_BASE=`echo ${DIST_BASE} | sed 's/ /\\\\ /g'`
ZIP_DIST=`echo $3 | sed 's/ /\\\\ /g'`
PY_EXEC=`echo $4 | sed 's/ /\\\\ /g'`
QT_PLUGINS=`echo $5 | sed 's/ /\\\\ /g'`

OS=`uname`

eval mkdir -p ${DIST_DIR}
BINARY_IN=${BIN_DIR}/blurator
BINARY_OUT=${DIST_DIR}/blurator

# Copy and manage dependencies.
if [ ${OS} = "Darwin" ]; then
    ${PY_EXEC} scripts/copy_deps.py ${BINARY_IN} ${DIST_DIR}
    if [ ! $? -eq 0 ]; then
        echo "Failed to copy deps!"
        exit -1
    fi
else
    eval cp ${BINARY_IN} ${BINARY_OUT}
    eval scripts/copyshared.sh ${BINARY_OUT} ${DIST_DIR}
fi

eval chmod +x ${BINARY_OUT}

# Copy Qt plugins.
eval mkdir -p ${DIST_DIR}/imageformats
if [ ${OS} = "Darwin" ]; then
    eval cp ${QT_PLUGINS}/imageformats/libqjpeg.dylib \
         ${QT_PLUGINS}/imageformats/libqgif.dylib \
         ${QT_PLUGINS}/imageformats/libqtiff.dylib \
         ${DIST_DIR}/imageformats/
    for plugin in ${DIST_DIR}/imageformats/*; do
        eval ${PY_EXEC} scripts/fix_binary.py ${plugin}
    done
else
    eval cp ${QT_PLUGINS}/imageformats/libqjpeg.so \
         ${QT_PLUGINS}/imageformats/libqgif.so \
         ${QT_PLUGINS}/imageformats/libqtiff.so \
         ${DIST_DIR}/imageformats/
fi

# Create zip file.
eval echo "Creating zip file: ${ZIP_DIST}"
eval cd ${BIN_DIR}
rm -f dist.zip
eval zip -r9q dist.zip ${DIST_BASE}
eval mv dist.zip ${ZIP_DIST}

echo "Cleaning up"
eval rm -fr ${DIST_DIR}
