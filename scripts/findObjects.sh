#!/usr/bin/env bash

SRC_FOLDER="src/main/c++"
TARGET_FOLDER="target/objects"
FILES=$(find ${SRC_FOLDER} -type f -name "*.c++")

for FILE in ${FILES}; do
    PACKAGE_FILE_EXT=${FILE#src/main/c++*}
    PACKAGE_FILE=${PACKAGE_FILE_EXT%.c++}
    OBJECT="${PACKAGE_FILE}.o++"
    #echo ${FILE}
    #echo ${PACKAGE_FILE_EXT}
    #echo ${PACKAGE_FILE}
    echo "${TARGET_FOLDER}${OBJECT}"
done

