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

#define FILTER_NAME1 1
#define FILTER_NAME2 2
#define FILTER_NAME3 3
#define FILTER_NAME4 4
#define FILTER_NAME5 5
#define FILTER_NAME6 6
#define FILTER_ITERATION_SIZE 9


typedef unsigned char P_SZ;

typedef struct {
  char version[PPM_VERSION_SIZE];
  int width;
  int height;
  int color_size;
  int image_size;
  P_SZ *image;
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
  result.image = (P_SZ *)malloc(result.image_size);
  fread(result.image, result.image_size, 1, f);
  fclose(f);
  return result;
}

void write_ppm_file(PPM_Struct ppm, P_SZ *dst, const char *file_path)
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
P_SZ clip_color(int color) {
  if (color > 255) return 255;
  if (color < 0) return 0;
  return (P_SZ) color;
}

P_SZ filter1(P_SZ p1,
             P_SZ p2,
             P_SZ p3,
             P_SZ p4,
             P_SZ p5,
             P_SZ p6,
             P_SZ p7,
             P_SZ p8,
             P_SZ p9) {

  return clip_color(p4 + 8 * p1 - p2 - p3 - p5 - p6 - p7 - p8 - p9);
}
P_SZ filter2(P_SZ p1,
             P_SZ p2,
             P_SZ p3,
             P_SZ p4,
             P_SZ p5,
             P_SZ p6,
             P_SZ p7,
             P_SZ p8,
             P_SZ p9) {

  return (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) / FILTER_ITERATION_SIZE;
    }
P_SZ filter3(P_SZ p1,
             P_SZ p2,
             P_SZ p3,
             P_SZ p4,
             P_SZ p5,
             P_SZ p6,
             P_SZ p7,
             P_SZ p8,
             P_SZ p9) {

  return (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) / FILTER_ITERATION_SIZE;
    }
P_SZ filter4(P_SZ p1,
             P_SZ p2,
             P_SZ p3,
             P_SZ p4,
             P_SZ p5,
             P_SZ p6,
             P_SZ p7,
             P_SZ p8,
             P_SZ p9) {
  return (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) / FILTER_ITERATION_SIZE;
    }
P_SZ filter5(P_SZ p1,
             P_SZ p2,
             P_SZ p3,
             P_SZ p4,
             P_SZ p5,
             P_SZ p6,
             P_SZ p7,
             P_SZ p8,
             P_SZ p9) {

  return (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) / FILTER_ITERATION_SIZE;
    }
P_SZ filter6(P_SZ p1,
             P_SZ p2,
             P_SZ p3,
             P_SZ p4,
             P_SZ p5,
             P_SZ p6,
             P_SZ p7,
             P_SZ p8,
             P_SZ p9) {
  return (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) / FILTER_ITERATION_SIZE;
    }

//------------------------------------
int filter_image(int width,
                 int height,
                 P_SZ *src,
                 P_SZ *dst,
                 int pixel_size,
                 int filter) {

  const int width_in_bytes = width * pixel_size;

  for (int y = 1; y < height - 1; ++y) {
    int pixel_offset = width_in_bytes * y;
    int pixel_offset_prev = width_in_bytes * (y - 1);
    int pixel_offset_next = width_in_bytes * (y + 1);


    for (int x = pixel_size; x < width_in_bytes - pixel_size; x += pixel_size) {

      int next_x = x + pixel_size;
      int prev_x = x - pixel_size;

      for (int i = 0; i < pixel_size; i ++) {

        P_SZ  p = src[pixel_offset + x + i];
        P_SZ  p_north              = src[pixel_offset_prev + x + i];
        P_SZ  p_north_east         = src[pixel_offset_prev + next_x + i];
        P_SZ  p_east               = src[pixel_offset + next_x + i];
        P_SZ  p_south_east         = src[pixel_offset_next + next_x + i];
        P_SZ  p_south              = src[pixel_offset_next + x + i];
        P_SZ  p_south_west         = src[pixel_offset_next + prev_x + i];
        P_SZ  p_west               = src[pixel_offset + prev_x + i];
        P_SZ  p_north_west         = src[pixel_offset_prev + prev_x + i];


        switch (filter) {
        case FILTER_NAME1:
          dst[pixel_offset + x + i] = filter1(p,
                                              p_north,
                                              p_north_east,
                                              p_east,
                                              p_south_east,
                                              p_south,
                                              p_south_west,
                                              p_west,
                                              p_north_west);
          break;
        case FILTER_NAME2:
          dst[pixel_offset + x + i] = filter2(p,
                                              p_north,
                                              p_north_east,
                                              p_east,
                                              p_south_east,
                                              p_south,
                                              p_south_west,
                                              p_west,
                                              p_north_west);
          break;
        case FILTER_NAME3:
          dst[pixel_offset + x + i] = filter3(p,
                                              p_north,
                                              p_north_east,
                                              p_east,
                                              p_south_east,
                                              p_south,
                                              p_south_west,
                                              p_west,
                                              p_north_west);
          break;
        case FILTER_NAME4:
          dst[pixel_offset + x + i] = filter4(p,
                                              p_north,
                                              p_north_east,
                                              p_east,
                                              p_south_east,
                                              p_south,
                                              p_south_west,
                                              p_west,
                                              p_north_west);
          break;
        case FILTER_NAME5:
          dst[pixel_offset + x + i] = filter5(p,
                                              p_north,
                                              p_north_east,
                                              p_east,
                                              p_south_east,
                                              p_south,
                                              p_south_west,
                                              p_west,
                                              p_north_west);
          break;
        case FILTER_NAME6:
          dst[pixel_offset + x + i] = filter6(p,
                                              p_north,
                                              p_north_east,
                                              p_east,
                                              p_south_east,
                                              p_south,
                                              p_south_west,
                                              p_west,
                                              p_north_west);
          break;
        default:
          dst[pixel_offset + x + i] = p;
        }
      }
    }
  }
  return 0;
}

//------------------------------------




int main() {

  char* input_file_name = "Lena.ppm";
  PPM_Struct ppm = read_ppm_file(input_file_name);
  P_SZ *dst;

  printf("VERSION=%s", ppm.version);
  printf("WIDTH=%d\n", ppm.width);
  printf("HEIGHT=%d\n", ppm.height);
  printf("COLOR_SIZE=%d\n", ppm.color_size);
  printf("IMAGE_SIZE=%d\n", ppm.image_size);

  dst = (P_SZ *) malloc(ppm.image_size);

  filter_image(ppm.width, ppm.height, ppm.image, dst, 3, FILTER_NAME1);
  write_ppm_file(ppm, dst, "out1.ppm");
  filter_image(ppm.width, ppm.height, ppm.image, dst, 3, FILTER_NAME2);
  write_ppm_file(ppm, dst, "out2.ppm");
  filter_image(ppm.width, ppm.height, ppm.image, dst, 3, FILTER_NAME3);
  write_ppm_file(ppm, dst, "out3.ppm");
  filter_image(ppm.width, ppm.height, ppm.image, dst, 3, FILTER_NAME4);
  write_ppm_file(ppm, dst, "out4.ppm");
  filter_image(ppm.width, ppm.height, ppm.image, dst, 3, FILTER_NAME5);
  write_ppm_file(ppm, dst, "out5.ppm");
  filter_image(ppm.width, ppm.height, ppm.image, dst, 3, FILTER_NAME6);
  write_ppm_file(ppm, dst, "out6.ppm");

  free(ppm.image);
  free(dst);
  return 0;
}
