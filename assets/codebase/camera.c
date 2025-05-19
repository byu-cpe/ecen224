#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "camera.h"
#include "log.h"

void camera_capture_data(uint8_t *buf, size_t bufsize) {
    char tmp_file_name[16] = "ecen-224-XXXXXX";
    char *command = "libcamera-still -n --immediate -e bmp --width 128 --height 128 -o ";

    // Create the temporary file
    int fp = mkstemp(tmp_file_name);

    // Create whole command
    ssize_t size = snprintf(NULL, 0, "%s%s", command, tmp_file_name);
    char *full_command = malloc(size + 1);
    snprintf(full_command, size + 1, "%s%s", command, tmp_file_name);

    // Run the command
    system(full_command);

    // Copy the data out of the file
    read(fp, buf, bufsize);

    // Clean up
    close(fp);
    remove(tmp_file_name);
    free(full_command);
}

void camera_save_to_file(uint8_t *buf, size_t bufsize, char *filename) {
    /*
    YOUR CODE HERE
    */
}
