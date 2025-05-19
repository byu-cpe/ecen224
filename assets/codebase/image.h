#include <stdint.h>
#include <stdio.h>

#define LOAD_ERROR 1
#define LOAD_SUCCESS 0
#define BMP_FILE_HEADER_SIZE 14
#define SAVE_ERROR 1
#define SAVE_SUCCESS 0

// Pixels in a bitmap file are arranged in the following format. Each pixel is
// three bytes of data, ordered by blue, green, red, with each color as one
// byte.
//
//               pixel x                   pixel x+1
//                   |                          |
//     +-------------+------------+-------------+------------+
//     v                          v                          v
//     +--------+--------+--------+--------+--------+--------+
// ... |   B    |   G    |   R    |   B    |   G    |   R    | ...
//     +--------+--------+--------+--------+--------+--------+
//       1 byte   1 byte   1 byte   1 byte   1 byte   1 byte
//
// To access the blue color in the xth pixel, you would index into pixel data by
// x * 3. To access the green color in the xth pixel, you would index into the
// pixel data by x * 3 + 1. To access the red color in the xth pixel, you would
// index into the pixel data by x * 3 + 2.

typedef enum Color { BLUE_CHANNEL, GREEN_CHANNEL, RED_CHANNEL } Color;

typedef struct {
    FILE *img;
    uint8_t file_header[BMP_FILE_HEADER_SIZE]; // 14 bytes
    uint8_t *dib_header;                       // Variable
    uint8_t *pxl_data;                         // Pixel data for image
    uint8_t *pxl_data_cpy;                     // Copy of pixel data
    uint8_t pxl_data_offset;                   // Location of pxl data in img
    uint32_t img_width;                        // Image width
    int32_t img_height;                        // Image height
    uint32_t file_size;                        // Size of the file
    uint32_t pxl_data_size;                    // Size of pixel data array
} Bitmap;

// --------------------------------------------------------------------------
// Image loading/saving functions
// --------------------------------------------------------------------------

// Given a buffer containing a BMP file (including the header), convert it to a Bitmap struct. This
// function allocates data on the heap. The caller must call destroy_bmp to free up the space.
//
//  dst - A pointer to the Bitmap struct that the data will be copied into. The memory *must be
//  already allocated*.
//  src - The source BMP file buffer (include the header).
uint8_t create_bmp(Bitmap *dst, uint8_t *src);

// Frees the space that a Bitmap struct allocates. For every call to create_bmp, there is should be
// a corresponding destroy_bmp call.
//
//  bmp - The bmp struct that will get deallocated.
void destroy_bmp(Bitmap *bmp);

// Resets the bitmap data to its original state. This should be used after
// modifying and saving the bitmap data to get the image back into its original
// state.
//
//  bmp - A pointer to Bitmap structure.
void reset_pixel_data(Bitmap *bmp);

// --------------------------------------------------------------------------
// Helper functions
// --------------------------------------------------------------------------

// Gets the pointer to the pixel data. This data can be modified. The save_img
// function uses this data to write a new image.
//
//  bmp - A pointer to the Bitmap structure.
uint8_t *get_pxl_data(Bitmap *bmp);

// Gets the pointer to the pixel data of the original image. This data should
// never be modified!
//
//  bmp - A pointer to the Bitmap structure.
uint8_t *get_original_pxl_data(Bitmap *bmp);

// --------------------------------------------------------------------------
// Image manipulation functions
// --------------------------------------------------------------------------

// Removes the indicated color from the bitmap data.
//
//  color - The color that should be removed (RED, GREEN, or BLUE)
//  bmp - A pointer to the Bitmap structure that contains the bitmap data
void remove_color_channel(Color color, Bitmap *bmp);

// Filters the image by or'ing vertically adjacent pixels in the bitmap data. For
// a given pixel, x, this function will OR the top and bottom pixels with x.
// Using the diagram below, the equation will be x | T | B. This function
// handles when on the top and bottom rows of the image.
//   +-+
//   |T|
// +-+-+-+
// |L|x|R|
// +-+-+-+
//   |B|
//   +-+
//
//  bmp - A pointer to the Bitmap structure that contains the bitmap data
void or_filter(Bitmap *bmp);
