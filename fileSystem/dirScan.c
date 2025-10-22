#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

void count_dir(const char *path, int *file_count, int *dir_count)
{
    DIR *dir;
    struct dirent *entry;
    char new_path[1024];
    struct stat info;

    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory '%s': %s\n", path, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
        if (stat(new_path, &info) == -1) {
            fprintf(stderr, "Error accessing '%s': %s\n", new_path, strerror(errno));
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            (*dir_count)++;
            count_dir(new_path, file_count, dir_count);
        } else if (S_ISREG(info.st_mode)) {
            (*file_count)++;
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    int file_count = 0, dir_count = 0;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    count_dir(argv[1], &file_count, &dir_count);
    printf("Total directories: %d\n", dir_count);
    printf("Total files: %d\n", file_count);

    return 0;
}
