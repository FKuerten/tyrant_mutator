#!/usr/bin/env bash
for MODULE in "$@"; do
    echo "../${MODULE}/target/lib${MODULE}.a"
done
