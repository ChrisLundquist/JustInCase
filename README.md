# JustInCase

Just in time case correction, for when you need it.
This makes a case sensitive file system a little less sensitive, and search for case insensitive matches when calling open.

```
$ make test
gcc -D _DARWIN_NO_64_BIT_INODE -Wall -std=gnu99 -fPIC -DPIC -g -c just_in_case.c -arch i386 -arch x86_64
gcc -D _DARWIN_NO_64_BIT_INODE -dynamiclib -o just_in_case.32.dylib just_in_case.o -ldl -current_version 1.0.0 -compatibility_version 1.0.0 -arch i386 -arch x86_64
gcc -Wall -std=gnu99 -fPIC -DPIC -g -c just_in_case.c -arch i386 -arch x86_64
gcc -dynamiclib -o just_in_case.64.dylib just_in_case.o -ldl -current_version 1.0.0 -compatibility_version 1.0.0 -arch x86_64 -arch i386
gcc -D _DARWIN_NO_64_BIT_INODE test.c -o test32 -arch i386
gcc test.c -o test64 -arch x86_64
./test.sh
32 bit test for _stat
loaded JustInCase into process: '89971', parent '89970'
failed stat for target: /Applications/Steam.app/Contents/MacOS/frameworks
Found replacement match: Frameworks
New path /Applications/Steam.app/Contents/MacOS/Frameworks
failed stat for target: /Applications/Steam.app/Contents/MacOS/fRameworks
Found replacement match: Frameworks
New path /Applications/Steam.app/Contents/MacOS/Frameworks
failed stat for target: /Applications/Steam.app/Contents/MacOS/fRAmeworks
Found replacement match: Frameworks
New path /Applications/Steam.app/Contents/MacOS/Frameworks

64 bit test for _stat$NODE64
loaded JustInCase into process: '89972', parent '89970'
failed stat for target: /Applications/Steam.app/Contents/MacOS/frameworks
Found replacement match: Frameworks
New path /Applications/Steam.app/Contents/MacOS/Frameworks
failed stat for target: /Applications/Steam.app/Contents/MacOS/fRameworks
Found replacement match: Frameworks
New path /Applications/Steam.app/Contents/MacOS/Frameworks
failed stat for target: /Applications/Steam.app/Contents/MacOS/fRAmeworks
Found replacement match: Frameworks
New path /Applications/Steam.app/Contents/MacOS/Frameworks
```
