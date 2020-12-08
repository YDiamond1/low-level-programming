#include "image_utils.h"

enum read_status read_from_bmp(FILE* in, image_t* const image){
    bmp_header_t*  header = malloc(sizeof(bmp_header_t));
    uint8_t rubbish[4];
    enum read_status header_check;
    int64_t row;


    if(image == NULL) return READ_IMAGE_NULL;
    if(!fread(header, sizeof(bmp_header_t), 1, in)) return READ_IO_HEADER;

    header_check = bmp_header_check(*header);
    if(header_check != READ_OK) return header_check;

    image->width = header->biWidth;
    image->height = header->biHeight;
    image->data = malloc(image->width * image->height * sizeof(pixel_t));

    int64_t remainder = (image->width*3) % 4;
    remainder = (remainder == 0) ? 0: 4-remainder;

    for(row = image->height-1; row>=0; row--){
        uint64_t bits_read = fread(&(image->data[row * image->width]), sizeof(pixel_t), image->width, in);
        uint64_t bits_rubbish = fread(rubbish, sizeof(uint8_t), remainder, in);
        if(!bits_read || (remainder && !bits_rubbish)) {
            free(image->data);
            return READ_IO_IMAGE;
        }
    }

    return READ_OK;

}

enum read_status bmp_header_check(bmp_header_t header){
    if(header.bfType != BF_TYPE_DEFAULT) return READ_INVALID_SIGNATURE;
    if(header.biBitCount != BI_BIT_COUNT_DEFAULT) return READ_INVALID_BITS;
    if(header.biCompression != BI_COMPRESSION_DEFAULT) return READ_INVALID_COMPRESSION;

    if(header.bfReserved != BF_RESERVED_DEFAULT) return READ_INVALID_HEADER;
    if(header.bOffBits != BF_OFF_BITS_DEFAULT) return READ_INVALID_HEADER;
    if(header.biSize != BI_SIZE_DEFAULT) return READ_INVALID_HEADER;
    if(header.biPlanes != BI_PLANES_DEFAULT) return READ_INVALID_HEADER;

    return READ_OK;
}
static void rotate_vector(double *new_x, double *new_y, double old_x, double old_y, double rad){
    *new_x = old_x * cos(rad) - old_y * sin(rad);
    *new_y = old_x * sin(rad) + old_y * cos(rad);

}
static double dmax4(double const v1, double const v2, double const v3, double const v4) {
    double max = v1;
    if (v2 > max) max = v2;
    if (v3 > max) max = v3;
    if (v4 > max) max = v4;
    return max;
}


static void calc_new_size(image_t* const result, image_t source, double rad){
    double right = source.width / 2.0;
    double left = -right;
    double top = source.height / 2.0;
    double bottom = -top;

    double lt_x, lt_y, rt_x, rt_y, lb_x, lb_y, rb_x, rb_y;;
    rotate_vector(&lt_x, &lt_y, left, top, rad);
    rotate_vector(&rt_x, &rt_y, right, top, rad);
    rotate_vector(&lb_x, &lb_y, left, bottom, rad);
    rotate_vector(&rb_x, &rb_y, right, bottom, rad);


    result->width = (uint64_t) round(dmax4(lt_x, rt_x, lb_x, rb_x) * 2 ) ;
    result->height = (uint64_t) round(dmax4(lt_y, rt_y, lb_y, rb_y) * 2 ) ;
}


image_t rotate(image_t* const source, int64_t angle){
    double rad = angle * M_PI / 180;
    double pivot_x = (double )(source->width-1) / 2.f , pivot_y = (double)(source->height-1) / 2.f;
    double new_pivot_x, new_pivot_y;


    image_t* result = malloc(sizeof(image_t));
    calc_new_size(result, *source, rad);
    result->data = malloc(result->width * result->height * sizeof(pixel_t));

    printf("result width - %ld height - %ld\n", result->width, result->height);

    new_pivot_x = (double)(result->width - 1)  / 2.f;
    new_pivot_y = (double)(result->height -1) / 2.f;

    for(uint64_t y = 0; y < result -> height; y++){
        for (uint64_t x = 0; x < result -> width; x++) {
            double old_x, old_y;
            double  index_old_x, index_old_y;
            rotate_vector(&old_x, &old_y, x-new_pivot_x, y-new_pivot_y, -rad);
            index_old_x = round(old_x + pivot_x);
            index_old_y = round(old_y + pivot_y);
            printf("old_x - %lf,pivot_x - %lf, old_y - %lf, pivot_y - %lf\n", old_x, pivot_x, old_y, pivot_y);
            if(index_old_x < 0 || index_old_x > source->width-1 || index_old_y < 0 || index_old_y > source->height-1) {
                //printf("index_old_x - %d, index_old_y - %d\n", index_old_x, index_old_y);
                result->data[y * result->width + x] = (pixel_t) {255, 255, 255};
            }
            else
                result->data[y * result->width + x] = source->data[(uint64_t)index_old_y * source->width + (uint64_t)index_old_x];
        }
    }

    return *result;
}

bmp_header_t bmp_header_collect(image_t image){
    bmp_header_t header;

    header.bfType = BF_TYPE_DEFAULT;
    header.biPlanes = BI_PLANES_DEFAULT;
    header.bfReserved = BF_RESERVED_DEFAULT;
    header.bOffBits = BF_OFF_BITS_DEFAULT;
    header.biCompression = BI_COMPRESSION_DEFAULT;
    header.biClrImportant = BI_CLR_IMPORTANT_DEFAULT;
    header.biBitCount = BI_BIT_COUNT_DEFAULT;
    header.biXPelsPerMeter = BI_X_PELS_PER_METER_DEFAULT;
    header.biYPelsPerMeter = BI_Y_PELS_PER_METER_DEFAULT;
    header.biClrUsed = BI_CLR_USED_DEFAULT;
    header.biSize = BI_SIZE_DEFAULT;

    header.biWidth = image.width;
    header.biHeight = image.height;

    uint64_t rubbish = (image.width * 3) % 4;
    rubbish = (rubbish == 0) ? 0 : 4 - rubbish;

    header.biSizeImage = (image.width * 3 + rubbish) * image.height;
    header.bfileSize = header.biSizeImage + header.bOffBits;

    return header;
}


enum write_status write_to_bmp(FILE* out, image_t* const image){
    const uint8_t rubbish[4] = {0};
    int64_t remainder, row;
    if(image == NULL) return WRITE_IMAGE_NULL;
    bmp_header_t header = bmp_header_collect(*image);
    if(fwrite(&header, sizeof(bmp_header_t), 1, out) != 1) return WRITE_IO_HEADER;

    remainder = (image->width * 3) % 4;
    remainder = remainder == 0 ? 0 : 4 - remainder;

    for (row = image->height -1 ; row >= 0 ; row--) {
        uint64_t row_bits = fwrite(&(image->data[row * image->width]), sizeof(pixel_t), image->width, out);
        uint64_t rubbish_bits = fwrite(rubbish, sizeof(uint8_t), remainder, out);
        if(!row_bits || (remainder && !rubbish_bits)) return WRITE_IO_IMAGE;
    }
    return WRITE_OK;
}

void perror_read(const char * msg, enum read_status error){
    if(error == READ_OK) return;
    char* err_mes;
    switch (error) {
        case READ_IMAGE_NULL: {
                err_mes = "Image didnt created";
                break;
            }
        case READ_INVALID_BITS:{
                err_mes = "Invalid bit count - only 24 bit images are supported";
                break;
            }
        case READ_INVALID_SIGNATURE:{
                err_mes = "Invalid signature - only bmp format are supported";
                break;
            }
        case READ_IO_HEADER:{
                err_mes = "Failed while reading a header from input file";
                break;
            }
        case READ_IO_IMAGE:{
                err_mes = "Failed while reading a image from input file";
                break;
            }
        case READ_INVALID_HEADER:{
                err_mes = "Invalid header values";
                break;
            }
        case READ_INVALID_COMPRESSION:{
            err_mes = "Invalid compression values - only uncompression pictures are supported";
            break;
        }
        default: {
            err_mes = "unknown failure";
            break;
        }

    }
    fprintf(stderr, "%s : %s\n",msg, err_mes);
}

void perror_write (const char * msg, enum write_status status){
    if(status == WRITE_OK) return;
    const char* err_msg;
    switch (status) {
        case WRITE_IMAGE_NULL: {
            err_msg = "Image didnt created";
            break;
        }
        case WRITE_IO_HEADER:{
            err_msg = "Failed while writing a header into output file";
            break;
        }
        case WRITE_IO_IMAGE:{
            err_msg = "Failed while writing a image into output file";
            break;
        }
        default:{
            err_msg = "Unknown failure";
            break;
        }

    }
    fprintf(stderr, "%s: %s\n", msg, err_msg);
}
void show(image_t* in) {
    struct pixel_t* bitMap = in->data;
    for (size_t i = 0; i < in->height; i ++){
        for (size_t j = 0; j < in->width; j ++)
            printf("pixel, R: %d, B: %d, G: %d\n", (*(bitMap + i  * in->width + j)).r, (*(bitMap + i * in->width + j)).b, (*(bitMap + i* in->width + j)).g);
        printf("\n");
    }
}
