#include <stdio.h>
#include "image_utils.h"

int main(int argc, char* argv[] ) {
    FILE *in = fopen(argv[1], "rb");
    FILE *out = fopen(argv[2], "wb");
    image_t image, image_res;
    int angle = 90;
    if(argc == 4){
        sscanf(argv[3], "%d", &angle);
    }
    printf("angle = %d\n", angle);
    if(!in || !out){
        printf("Couldn't open files\n");
        return 0;
    }
    enum read_status read = read_from_bmp(in, &image);
    if(read != READ_OK){
        perror_read("Error while deserializing a bmp file", read);
        return 1;
    }
    show(&image);
    image_res = rotate(&image,angle);
    printf("---------------------------------\n");
    show(&image_res);

    enum write_status write = write_to_bmp(out, &image_res);
    if(write != WRITE_OK){
        perror_write("Error while serializing a bmp file", write);
        return 1;
    }
    if(fclose(in) || fclose(out)){
        printf("Couldn't close files");
        return 1;
    }

    free(image_res.data);
    free(image.data);
    return 0;
}
