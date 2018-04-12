just_in_case.dylib : just_in_case.o
	gcc -dynamiclib -o just_in_case.dylib just_in_case.o -ldl -arch i386 -arch x86_64

just_in_case.o: just_in_case.c
	gcc -Wall -std=gnu99 -fPIC -DPIC -g -c just_in_case.c -arch i386 -arch x86_64

test_bin: test.c
	gcc test.c -o test

test: test_bin
	./test.sh
