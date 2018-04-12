#include <dlfcn.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/attr.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define EXPORT __attribute__((visibility("default")))

#define DYLD_INTERPOSE(_replacment,_replacee) \
__attribute__((used)) static struct{ const void* replacment; const void* replacee; } _interpose_##_replacee \
__attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacment, (const void*)(unsigned long)&_replacee };

static inline bool target_path(const char *path) {
    return strcasestr(path, "steam");
}

EXPORT
int jic_open(const char *path, int oflag, ...) {
    va_list ap;
    int mode;

	if ((oflag & O_CREAT) != 0) {
		va_start(ap, oflag);
		mode = va_arg(ap, int);
		va_end(ap);
	} else {
		mode = 0;
	}

    int fd = open(path, oflag, mode);
    if (fd < 0) {
        fprintf(stderr, "**************\n");
        fprintf(stderr, "Failed to open: %s\n", path);
        fprintf(stderr, "**************\n");
    }
    return fd;
}

/*
EXPORT
FILE* jic_fopen(const char * restrict path, const char * restrict mode) {
    FILE* ret = fopen(path, mode);
    if (ret < 0) {
        fprintf(stderr, "**************\n");
        fprintf(stderr, "Failed to fopen: %s\n", path);
        fprintf(stderr, "**************\n");
    }
    return ret;
}
*/

EXPORT
int jic_stat(const char *restrict path, struct stat *restrict buf) {
    int ret = stat(path, buf);
    if (target_path(path)) {
        fprintf(stderr, "**************\n");
        fprintf(stderr, "got stat for target: %s\n", path);
        fprintf(stderr, "**************\n");
    }
    if (ret < 0) {
        fprintf(stderr, "**************\n");
        fprintf(stderr, "Failed to stat: %s\n", path);
        fprintf(stderr, "**************\n");
    }
    return ret;
}

/*
EXPORT
int jic_statvfs(const char *restrict path, struct statvfs *restrict buf) {
    int ret = statvfs(path, buf);
    if (target_path(path)) {
        fprintf(stderr, "**************\n");
        fprintf(stderr, "got statvsf for target: %s\n", path);
        fprintf(stderr, "**************\n");
    }
    if (ret < 0) {
        fprintf(stderr, "**************\n");
        fprintf(stderr, "Failed to statvfs: %s\n", path);
        fprintf(stderr, "**************\n");
    }
    return ret;
}
*/

EXPORT
int jic_getattrlist(const char* path, struct attrlist * attrList, void * attrBuf, size_t attrBufSize, unsigned long options) {
    //int mask = ~(VOL_CAP_FMT_CASE_SENSITIVE | VOL_CAP_FMT_CASE_PRESERVING);
    if (target_path(path)) {
        fprintf(stderr, "**************\n");
        fprintf(stderr, "hijacked getattrlist for: %s\n", path);
        fprintf(stderr, "**************\n");
        memset(attrList, 0, sizeof(*attrList));
        memset(attrBuf, 0, attrBufSize);
        return -1;
    } else {
        int result = getattrlist(path, attrList, attrBuf, attrBufSize, options);
        return result;
    }

}

// Initializer.
__attribute__((constructor))
static void initializer(void) {                             // 2
    fprintf(stderr, "loaded shared lib\n");
}

//DYLD_INTERPOSE(jic_fopen, fopen);
DYLD_INTERPOSE(jic_open, open);
DYLD_INTERPOSE(jic_stat, stat);
//DYLD_INTERPOSE(jic_statvfs, statvfs);
DYLD_INTERPOSE(jic_getattrlist, getattrlist);
