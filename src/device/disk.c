#include <uefi.h>
#include "device/disk.h"

void dir(char *path) {
    DIR *dh;
    struct dirent *de;
    if((dh = opendir(path))) {
        printf("content of %s directory:\n", path);
        while ((de = readdir(dh)) != NULL) {
            printf("%s %x %s\n", de->d_type == DT_DIR ? "<dir> " : "<file>", de->d_type, de->d_name);
        }
        closedir(dh);
    } else
        printf("unable to open \"%s\" directory\n", path);
}