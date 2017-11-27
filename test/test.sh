#!/usr/bin/env bash

# find location of this script, move one level up
ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && cd .. && pwd )"

# set up env vars
export HOUDINI_DSO_PATH="${HOUDINI_DSO_PATH}:&:$ROOT/dso"
export HOUDINI_DSO_ERROR=1
echo "DSO path "$HOUDINI_DSO_PATH

# run testing scene
houdini -foreground $ROOT/test/test.hipnc