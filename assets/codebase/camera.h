#include <stdint.h>

#define IMG_SIZE 49206

/*
 * Takes a picture using the camera. It returns the full image *with* the BMP header. This buffer
 * can not be used in some functions that are expecting only the pixel data, such as
 * display_draw_image_data in display.h.
 *
 * uint8_t * buf: a buffer where the image data of the photo taken will be stored
 * size_t bufsize: integer that holds the size of the buffer. The size of the photo
 *                 being taken is found as a #DEFINEd variable in "camera.h"
 */
void camera_capture_data(uint8_t *buf, size_t bufsize);

/*
 * Takes image data *with* the BMP header and saves it to a file.
 *
 * uint8_t * buf: a buffer where the image data of the photo taken is stored
 * size_t bufsize: integer that holds the size of the buffer. The size of the photo
 *                 being taken is found as a #DEFINEd variable in "camera.h"
 * char * filename: name of the file that is being saved
 */
void camera_save_to_file(uint8_t *buf, size_t bufsize, char *filename);
