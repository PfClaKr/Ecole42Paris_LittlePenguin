#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define DEVICE_PATH "/dev/fortytwo"
#define USER_LOGIN "ychun"
#define BUFFER_SIZE 32

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device file");
        return errno;
    }

    bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("Failed to read from the device file");
        close(fd);
        return errno;
    }
    buffer[bytes_read] = '\0';
    printf("Read from device: %s\n", buffer);
    close(fd);

    fd = open(DEVICE_PATH, O_RDWR);
    bytes_written = write(fd, USER_LOGIN, strlen(USER_LOGIN));
    if (bytes_written < 0) {
        perror("Failed to write the correct login to the device file");
        close(fd);
        return errno;
    }
    printf("Successfully wrote the correct login to the device.\n");
    close(fd);


    fd = open(DEVICE_PATH, O_RDWR);
    bytes_written = write(fd, "wronglogin", strlen("wronglogin"));
    if (bytes_written < 0) {
        perror("Failed to write the incorrect login to the device file");
    } else {
        printf("Unexpectedly succeeded in writing the incorrect login to the device.\n");
    }

    close(fd);
    return 0;
}
