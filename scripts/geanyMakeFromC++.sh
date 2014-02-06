#!/usr/bin/env bash

#assume $1 contains a filename
SOURCEFILE="$1"
#echo ${SOURCEFILE}
SOURCEFILE_FULL=$(readlink -e ${SOURCEFILE})
SEARCH="src/main/c++/"
REPLACE="target/objects/"
TEMP="${SOURCEFILE_FULL/${SEARCH}/${REPLACE}}"
OBJECTFILE_FULL="${TEMP/.c++/.o++}"
OBJECTFILE_FULL="${OBJECTFILE_FULL/.h++/.o++}"
#echo ${OBJECTFILE_FULL}
DIR="$PWD"
#echo $DIR
OBJECTFILE=$(python -c "import os.path; print os.path.relpath('${OBJECTFILE_FULL}','${DIR}')")
#echo ${OBJECTFILE}

echo makepp ${OBJECTFILE}
exec makepp ${OBJECTFILE}
