#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int f = open("/dev/null", O_RDONLY);
    if (f < 0) {
        fprintf(stderr, "failed opening /dev/null\n");
        return -1;
    }
    close(f);

    struct stat buf;
    if(stat("/Applications/Steam.app/Contents/MacOS/Frameworks\0", &buf) < 0) {
        fprintf(stderr, "failed Frameworks\n");
        return -1;
    }
    if (stat("/Applications/Steam.app/Contents/MacOS/frameworks\0", &buf) < 0) {
        fprintf(stderr, "failed frameworks\n");
        return -1;
    }
    if (stat("/Applications/Steam.app/Contents/MacOS/fRameworks\0", &buf) < 0) {
        fprintf(stderr, "failed fRameworks\n");
        return -1;
    }
    if (stat("/Applications/Steam.app/Contents/MacOS/fRAmeworks\0", &buf) < 0) {
        fprintf(stderr, "failed fRAmeworks\n");
        return -1;
    }
    return 0;
}
