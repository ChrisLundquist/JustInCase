just_in_case.32.dylib : just_in_case.32.o
	gcc -D _DARWIN_NO_64_BIT_INODE -dynamiclib -o just_in_case.32.dylib just_in_case.o -ldl -current_version 1.0.0 -compatibility_version 1.0.0 -arch i386 -arch x86_64

just_in_case.64.dylib : just_in_case.64.o
	gcc -dynamiclib -o just_in_case.64.dylib just_in_case.o -ldl -current_version 1.0.0 -compatibility_version 1.0.0 -arch x86_64 -arch i386

just_in_case.32.o: just_in_case.c
	gcc -D _DARWIN_NO_64_BIT_INODE -Wall -std=gnu99 -fPIC -DPIC -g -c just_in_case.c -arch i386 -arch x86_64

just_in_case.64.o: just_in_case.c
	gcc -Wall -std=gnu99 -fPIC -DPIC -g -c just_in_case.c -arch i386 -arch x86_64

test_bin: test.c just_in_case.32.dylib just_in_case.64.dylib
	gcc -D _DARWIN_NO_64_BIT_INODE test.c -o test32 -arch i386
	gcc test.c -o test64 -arch x86_64

test: test_bin
	./test.sh
