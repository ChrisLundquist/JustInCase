#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/attr.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <unistd.h>

#define EXPORT __attribute__((visibility("default")))

#define DYLD_INTERPOSE(_replacment,_replacee) \
    __attribute__((used)) static struct{ const void* replacment; const void* replacee; } _interpose_##_replacee \
    __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacment, (const void*)(unsigned long)&_replacee };

static inline bool target_path(const char *path) {
    return (strcasestr(path, "steam") || strcasestr(path, "public"));
}

// Caller must free
char* find_replacement(const char *restrict path) {
    char* tmp_path = strdup(path);
    char* dir_name = dirname(tmp_path);
    char* file_name = basename(tmp_path);
    //fprintf(stderr, "path %s\n", tmp_path);
    //fprintf(stderr, "dir %s\n", dir_name);
    //fprintf(stderr, "file %s\n", file_name);
    struct dirent *ent;
    int file_len = strlen(file_name);
    int dir_len = strlen(dir_name);

    DIR* dir = opendir(dir_name);
    if (dir == NULL)
        return tmp_path; // forward whatever we had

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_namlen == file_len && strcasecmp(ent->d_name, file_name) == 0) {
            fprintf(stderr, "Found replacement match: %s\n", ent->d_name);
            strcpy(tmp_path + dir_len + 1, ent->d_name); // + 1 for trailing slash
            fprintf(stderr, "New path %s\n", tmp_path);
            break;
        }
    }
    (void)closedir(dir);
    return tmp_path;
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
    if (fd < 0 ) { //&& target_path(path)) {
        fprintf(stderr, "failed to open: %s\n", path);
        char* replacement_path = find_replacement(path);
        fd = open(replacement_path, oflag, mode);
        free(replacement_path);
    }

    return fd;
}

/*
EXPORT
FILE* jic_fopen(const char * restrict path, const char * restrict mode) {
    FILE* ret = fopen(path, mode);
    if (ret < 0 && target_path(path)) {
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
    if (ret < 0 && target_path(path)) {
        fprintf(stderr, "failed stat for target: %s\n", path);
        char* replacement_path = find_replacement(path);
        ret = stat(replacement_path, buf);
        free(replacement_path);
    }
    return ret;
}

EXPORT
int jic_getattrlist(const char* path, struct attrlist * attrList, void * attrBuf, size_t attrBufSize, unsigned long options) {
    //int mask = ~(VOL_CAP_FMT_CASE_SENSITIVE | VOL_CAP_FMT_CASE_PRESERVING);
    int result = getattrlist(path, attrList, attrBuf, attrBufSize, options);
    if (result >= 0 && target_path(path)) {
        if ( attrBufSize == 12)  {
            fprintf(stderr, "**************\n");
            fprintf(stderr, "hijacked getattrlist for target: %s\n", path);
            /*
               fprintf(stderr, "attr count: %d\n", attrList->bitmapcount);
               fprintf(stderr, "commonattr: %d\n", attrList->commonattr);
               fprintf(stderr, "volattr: %d\n", attrList->volattr);
               fprintf(stderr, "dirattr: %d\n", attrList->dirattr);
               fprintf(stderr, "fileattr: %d\n", attrList->fileattr);
               fprintf(stderr, "forkattr: %d\n", attrList->forkattr);
               fprintf(stderr, "attrBufSize: %zu\n", attrBufSize); */
            //bool case_sensitive = (attrBuf.cap.valid[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_CASE_SENSITIVE) && (attrBuf.cap.capabilities[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_CASE_SENSITIVE);
            fprintf(stderr, "**************\n");
            //memset(attrList, 0, sizeof(*attrList));
            memset(attrBuf, 0, attrBufSize);
            ((int*)attrBuf)[0] = sizeof(int);
        }
    }
    return result;

}

// Initializer.
__attribute__((constructor))
static void initializer(void) {
    fprintf(stderr, "loaded JustInCase into process: '%d', parent '%d'\n", getpid(), getppid());
}

//DYLD_INTERPOSE(jic_fopen, fopen);
DYLD_INTERPOSE(jic_open, open);
DYLD_INTERPOSE(jic_stat, stat);
DYLD_INTERPOSE(jic_getattrlist, getattrlist);
