#!/bin/bash
#export X=1
#export DYLD_PRINT_LIBRARIES=1
export DYLD_INSERT_LIBRARIES=/Users/clundquist/hack/steam-case-fix/just_in_case.32.dylib
echo "32 bit test for _stat"
./test32
echo ""
export DYLD_INSERT_LIBRARIES=/Users/clundquist/hack/steam-case-fix/just_in_case.64.dylib
echo "64 bit test for _stat\$NODE64"
./test64
