#!/bin/sh
# Usage: makedist.sh <binary dir> <dist dir> <zip dist>

BIN_DIR=`echo $1 | sed 's/ /\\\\ /g'`
DIST_DIR=`echo $2 | sed 's/ /\\\\ /g'`
DIST_BASE=`eval basename ${DIST_DIR}`
DIST_BASE=`echo ${DIST_BASE} | sed 's/ /\\\\ /g'`
ZIP_DIST=`echo $3 | sed 's/ /\\\\ /g'`
PY_EXEC=`echo $4 | sed 's/ /\\\\ /g'`

eval mkdir -p ${DIST_DIR}
BINARY_IN=${BIN_DIR}/blurator
BINARY_OUT=${DIST_DIR}/blurator

# Copy and manage dependencies.
${PY_EXEC} scripts/copy_deps.py ${BINARY_IN} ${DIST_DIR}
if [ ! $? -eq 0 ]; then
   echo "Failed to copy deps!"
   exit -1
fi

chmod +x ${BINARY_OUT}

# Create zip file.
eval echo "Creating zip file: ${ZIP_DIST}"
cd ${BIN_DIR}
rm -f dist.zip
eval zip -r9q dist.zip ${DIST_BASE}
eval mv dist.zip ${ZIP_DIST}

echo "Cleaning up"
eval rm -fr ${DIST_DIR}
