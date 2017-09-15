#!/usr/bin/env bash

# find location of this script
ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# set up env vars
HOUDINI_DSO_PATH="${HOUDINI_DSO_PATH}:&:$ROOT/dso"
HOUDINI_DSO_ERROR=1

# run testing scene
houdini -foreground $ROOT/test.hip
