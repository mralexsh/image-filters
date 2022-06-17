#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#include <sys/stat.h>
#include <sys/types.h>



#define PPM_VERSION_SIZE 80
#define LINE_SIZE 80

typedef struct {
    char version[PPM_VERSION_SIZE];
    int width;
    int height;
    int color_size;
    int image_size;
    char *image;
} PPM_Struct;


PPM_Struct read_ppm_file(char *file_path) {
    PPM_Struct result;
    char width_height_str[LINE_SIZE];
    char pixel_factor[LINE_SIZE];
    char *token;
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: could not open file %s: %m\n",
                file_path);
        exit(1);
    }

    if (fgets(result.version, PPM_VERSION_SIZE, f) == NULL) {
        fprintf(stderr, "ERROR: could not read  header\n");
        exit(1);
    }

    if (fgets(width_height_str, LINE_SIZE, f) == NULL) {
        fprintf(stderr, "ERROR: could not read  width and height\n");
        exit(1);
    }

    if (fgets(pixel_factor, LINE_SIZE, f) == NULL) {
        fprintf(stderr, "ERROR: could not read  pixel factor\n");
        exit(1);
    }

    token = strtok(width_height_str, " ");

    if (token == NULL) {
        fprintf(stderr, "ERROR: could not parse  width \n");
        exit(1);
    }

    result.width = atoi(token);

    token = strtok(NULL, " ");

    if (token == NULL) {
        fprintf(stderr, "ERROR: could not parse  height \n");
        exit(1);
    }
    result.height = atoi(token);

    //TODO handle pixel size
    result.color_size = 3;
    result.image_size = result.width * result.height * result.color_size;
    result.image = (char *)malloc(result.image_size);
    fread(result.image, result.image_size, 1, f);
    fclose(f);
    return result;
}

void write_ppm_file(PPM_Struct ppm, char *dst, const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: could not open file %s: %m\n",
                file_path);
        exit(1);
    }

    fprintf(f, "P6\n%d %d\n255\n", ppm.width, ppm.height);
    fwrite(dst, 1, ppm.image_size, f);
    fclose(f);
}

//------------------------------------
int test_image_filter(int width, int height, char *src, char *dst) {
    int offset = 0;
    for (int y = 0; y < height; ++y) {
        for(int x = 0; x < width * 3; x += 3) {
            dst[width * 3 * y + x] = 0;
            dst[width * 3 * y + x + 1] = 0;
            dst[width * 3 * y + x + 2] = offset;
        }
        if (y % (height / 255) == 0) offset ++;
        if (offset > 255) offset = 0;
    }
    return 0;
}
//------------------------------------
int test_image_copy(char *src, char *dst, int size) {
    for (int i = 0; i < size; i += 3) {
        dst[i] = src[i];
        dst[i + 1] = src[i + 1];
        dst[i + 2] = src[i + 2];
    }
    return 0;
}
unsigned char clip_color(int color) {
  return (unsigned char) color;
}
//------------------------------------
int moving_average_filter(int width, int height, char *src, char *dst, int pixel_size) {
    const  char window_size = 9;
    const int width_in_bytes = width * pixel_size;

    for (int y = 1; y < height - 1; ++y) {
        int pixel_offset = width_in_bytes * y;
        int pixel_offset_prev = width_in_bytes * (y - 1);
        int pixel_offset_next = width_in_bytes * (y + 1);


        for(int x = pixel_size; x < width_in_bytes - pixel_size; x += pixel_size) {

            int next_x = x + pixel_size;
            int prev_x = x - pixel_size;

            unsigned char red = src[pixel_offset + x];
            unsigned char green = src[pixel_offset + x + 1];
            unsigned char blue = src[pixel_offset + x + 2];

            unsigned char red_north              = src[pixel_offset_prev + x];
            unsigned char green_north            = src[pixel_offset_prev + x + 1];
            unsigned char blue_north             = src[pixel_offset_prev + x + 2];

            unsigned char red_north_east         = src[pixel_offset_prev + next_x];
            unsigned char green_north_east       = src[pixel_offset_prev + next_x + 1];
            unsigned char blue_north_east        = src[pixel_offset_prev + next_x + 2];

            unsigned char red_east               = src[pixel_offset + next_x];
            unsigned char green_east             = src[pixel_offset + next_x + 1];
            unsigned char blue_east              = src[pixel_offset + next_x + 2];

            unsigned char red_south_east         = src[pixel_offset_next + next_x];
            unsigned char green_south_east       = src[pixel_offset_next + next_x + 1];
            unsigned char blue_south_east        = src[pixel_offset_next + next_x + 2];

            unsigned char red_south              = src[pixel_offset_next + x];
            unsigned char green_south            = src[pixel_offset_next + x + 1];
            unsigned char blue_south             = src[pixel_offset_next + x + 2];

            unsigned char red_south_west         = src[pixel_offset_next + prev_x];
            unsigned char green_south_west       = src[pixel_offset_next + prev_x + 1];
            unsigned char blue_south_west        = src[pixel_offset_next + prev_x + 2];

            unsigned char red_west               = src[pixel_offset + prev_x];
            unsigned char green_west             = src[pixel_offset + prev_x + 1];
            unsigned char blue_west              = src[pixel_offset + prev_x + 2];

            unsigned char red_north_west         = src[pixel_offset_prev + prev_x];
            unsigned char green_north_west       = src[pixel_offset_prev + prev_x + 1];
            unsigned char blue_north_west        = src[pixel_offset_prev + prev_x + 2];

            dst[pixel_offset + x] = clip_color((red +
                                           red_north +
                                           red_north_east +
                                           red_east +
                                           red_south_east +
                                           red_south +
                                           red_south_west +
                                           red_west +
                                            red_north_west) / window_size);
        dst[pixel_offset + x + 1] = clip_color((green +
                                               green_north +
                                               green_north_east +
                                               green_east +
                                               green_south_east +
                                               green_south +
                                               green_south_west +
                                               green_west +
                                            green_north_west) / window_size);
        dst[pixel_offset + x + 2] = clip_color((blue +
                                               blue_north +
                                               blue_north_east +
                                               blue_east +
                                               blue_south_east +
                                               blue_south +
                                               blue_south_west +
                                               blue_west +
                                            blue_north_west) / window_size);
        }

    }
    return 0;
}

//------------------------------------




int main() {

  char* input_file_name = "123.ppm";
  PPM_Struct ppm = read_ppm_file(input_file_name);
  char *dst;

  printf("VERSION=%s", ppm.version);
  printf("WIDTH=%d\n", ppm.width);
  printf("HEIGHT=%d\n", ppm.height);
  printf("COLOR_SIZE=%d\n", ppm.color_size);
  printf("IMAGE_SIZE=%d\n", ppm.image_size);

  dst = (char *) malloc(ppm.image_size);
  moving_average_filter(ppm.width, ppm.height, ppm.image, dst, 3);
  //    test_image_copy(ppm.image, dst, ppm.image_size);
  write_ppm_file(ppm, dst, "out.ppm");
  free(ppm.image);
  free(dst);
  return 0;
}
