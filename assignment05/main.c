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

    // 1. Open the device file
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device file");
        return errno;
    }

    // 2. Test reading from the device
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("Failed to read from the device file");
        close(fd);
        return errno;
    }
    buffer[bytes_read] = '\0'; // Null-terminate the string
    printf("Read from device: %s\n", buffer);

    // 3. Test writing the correct login to the device
    bytes_written = write(fd, USER_LOGIN, strlen(USER_LOGIN));
    if (bytes_written < 0) {
        perror("Failed to write the correct login to the device file");
        close(fd);
        return errno;
    }
    printf("Successfully wrote the correct login to the device.\n");

    // 4. Test writing an incorrect login to the device
    bytes_written = write(fd, "wronglogin", strlen("wronglogin"));
    if (bytes_written < 0) {
        perror("Failed to write the incorrect login to the device file");
    } else {
        printf("Unexpectedly succeeded in writing the incorrect login to the device.\n");
    }

    // 5. Close the device file
    close(fd);
    return 0;
}
