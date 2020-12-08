

#ifndef LABA6_IMAGE_UTILS_H
#define LABA6_IMAGE_UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static const uint16_t BF_TYPE_DEFAULT = 0x4d42; /* BMP type, little-endian */
static const uint32_t BF_RESERVED_DEFAULT = 0;
static const uint32_t BF_OFF_BITS_DEFAULT = 54; /* total BMPv3 header size */
static const uint16_t BI_PLANES_DEFAULT = 1;
static const uint32_t BI_SIZE_DEFAULT = 40; /* total size of BMPv3 specific values */

/* Defaults specific for this task */
static const uint16_t BI_BIT_COUNT_DEFAULT = 24;
static const uint32_t BI_COMPRESSION_DEFAULT = 0;
static const uint32_t BI_X_PELS_PER_METER_DEFAULT = 0;
static const uint32_t BI_Y_PELS_PER_METER_DEFAULT = 0;
static const uint32_t BI_CLR_USED_DEFAULT = 0;
static const uint32_t BI_CLR_IMPORTANT_DEFAULT = 0;


typedef struct __attribute__((packed)) bmp_header {
        uint16_t bfType;
        uint32_t bfileSize;
        uint32_t bfReserved;
        uint32_t bOffBits;
        uint32_t biSize;
        uint32_t biWidth;
        uint32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
} bmp_header_t;

typedef struct pixel_t{
    uint8_t b,g,r;
}pixel_t;

typedef struct image{
    uint64_t width, height;
    pixel_t * data;
}image_t;
enum read_status{
    READ_OK=0,
    READ_IMAGE_NULL,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_IO_HEADER,
    READ_IO_IMAGE,
    READ_INVALID_COMPRESSION
};

enum write_status{
    WRITE_OK=0,
    WRITE_IMAGE_NULL,
    WRITE_IO_HEADER,
    WRITE_IO_IMAGE
};

enum read_status read_from_bmp(FILE* in, image_t* const image);

enum read_status bmp_header_check(bmp_header_t header);

enum write_status write_to_bmp(FILE* out, image_t* const image);

void perror_read(const char * msg, enum read_status error);

void perror_write(const char * msg , enum write_status status);

image_t rotate(image_t* const source, int64_t angle);

void show(image_t* source);
#endif //LABA6_IMAGE_UTILS_H
