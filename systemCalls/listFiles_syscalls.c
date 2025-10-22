#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char path[1024];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    dir = opendir(argv[1]); 
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory '%s': %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", argv[1], entry->d_name);
        if (stat(path, &file_stat) == -1) {
            fprintf(stderr, "Error getting info for '%s': %s\n", path, strerror(errno));
            continue;
        }

        printf("%s\t%ld bytes\n", entry->d_name, file_stat.st_size);
    }

    if (closedir(dir) == -1) {
        fprintf(stderr, "Error closing directory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}
