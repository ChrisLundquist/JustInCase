#!/bin/bash
#export X=1
#export DYLD_PRINT_LIBRARIES=1
export DYLD_INSERT_LIBRARIES=/Users/clundquist/hack/steam-case-fix/just_in_case.32.dylib:/Users/clundquist/hack/steam-case-fix/just_in_case.64.dylib
echo "32 bit test"
./test32

echo "64 bit test"
./test64
