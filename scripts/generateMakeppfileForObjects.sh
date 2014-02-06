#!/usr/bin/env bash

SRC_FOLDER="src/main/c++"
FOLDERS=$(find ${SRC_FOLDER} -type d | grep -v "/\.")
MAKEPPFILE_TEMPLATE="scripts/MakeppfileForObjectsTemplate"

for FOLDER in ${FOLDERS}; do
    PACKAGE=${FOLDER#src/main/c++*}
    #echo ${FOLDER}
    #echo ${PACKAGE}
    sed <${MAKEPPFILE_TEMPLATE} --expression="s!#PACKAGE#!${PACKAGE}!g"
done
