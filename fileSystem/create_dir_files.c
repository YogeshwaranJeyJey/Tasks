#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main()
{
    const char *dir_name = "sample_dir";
    const char *files[] = {"file1.txt", "file2.txt", "file3.txt"};
    const char *text[] = {"This is file 1\n", "This is file 2\n", "This is file 3\n"};
    char path[256], buffer[256];
    int fd;
    ssize_t bytes_read;

    if (mkdir(dir_name, 0755) == -1 && errno != EEXIST) {
        fprintf(stderr, "Error creating directory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        snprintf(path, sizeof(path), "%s/%s", dir_name, files[i]);
        fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            fprintf(stderr, "Error creating file '%s': %s\n", path, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (write(fd, text[i], strlen(text[i])) == -1) {
            fprintf(stderr, "Error writing to '%s': %s\n", path, strerror(errno));
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
    }

    printf("Files created and written successfully.\n\nReading contents:\n\n");

    for (int i = 0; i < 3; i++) {
        snprintf(path, sizeof(path), "%s/%s", dir_name, files[i]);
        fd = open(path, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Error opening file '%s': %s\n", path, strerror(errno));
            exit(EXIT_FAILURE);
        }
        printf("%s: ", files[i]);
        while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
        }
        close(fd);
    }

    return 0;
}
