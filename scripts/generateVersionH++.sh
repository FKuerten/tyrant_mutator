#!/usr/bin/env bash
echo -n "#define TYRANT_MUTATOR_VERSION \""
git rev-parse HEAD | tr --delete "\n"
echo  "\""
echo -n "#define TYRANT_MUTATOR_VERSION_TAGS \""
git tag --points-at HEAD | tr "\n" " "
echo  "\""
echo -n "#define TYRANT_MUTATOR_VERSION_DESCRIBE \""
git describe HEAD | tr --delete "\n"
echo "\""
#
echo -n "#define TYRANT_MUTATOR_DIRTY_HEAD "
if [ `git diff HEAD | wc --chars` -gt 0 ]; then
    echo true
else \
    echo false
fi
echo -n "#define TYRANT_MUTATOR_DIRTY_HASH \""
git diff HEAD | sha1sum | cut -d " " -f 1 | tr -d "\n"
echo  "\""
#

