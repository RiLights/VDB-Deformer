#!/usr/bin/env bash

ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# set up env vars
export HOUDINI_DSO_PATH="&:$ROOT/dso"
export HOUDINI_DSO_ERROR=1

cd $ROOT
houdini -foreground test.hip
