#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        fprintf(stderr, "Error opening source file '%s': %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        fprintf(stderr, "Error opening/creating destination file '%s': %s\n", argv[2], strerror(errno));
        close(src_fd);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(src_fd, buffer, BUF_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            fprintf(stderr, "Error writing to '%s': %s\n", argv[2], strerror(errno));
            close(src_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        fprintf(stderr, "Error reading from '%s': %s\n", argv[1], strerror(errno));
        close(src_fd);
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    if (close(src_fd) == -1)
        fprintf(stderr, "Error closing source file: %s\n", strerror(errno));

    if (close(dest_fd) == -1) {
        fprintf(stderr, "Error closing destination file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("File copied successfully from '%s' to '%s'.\n", argv[1], argv[2]);
    return 0;
}
