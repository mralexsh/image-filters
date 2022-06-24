#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "image-filters.h"



#define LINE_SIZE 80


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

//------------------------------------




int main() {

  char out_file_name[80];
  char* input_file_name = "123.ppm";
  //char* input_file_name = "Lena.ppm";
  PPM_Struct ppm = read_ppm_file(input_file_name);
  P_SZ *dst;

  printf("VERSION=%s", ppm.version);
  printf("WIDTH=%d\n", ppm.width);
  printf("HEIGHT=%d\n", ppm.height);
  printf("COLOR_SIZE=%d\n", ppm.color_size);
  printf("IMAGE_SIZE=%d\n", ppm.image_size);
  double total_time_spent = 0.0;
  dst = (P_SZ *) malloc(ppm.image_size);
  for (int i = 1; i <= FILTERS_AMOUNT; ++i) {
    double time_spent = 0.0;
    clock_t begin = clock();
    filter_image(ppm.width, ppm.height, ppm.image, dst, 3, i);
    clock_t end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    total_time_spent += time_spent;
    printf("Filter #%d. Execution time: %f\n", i, time_spent);
    sprintf(out_file_name,"filter_out%d.ppm", i);
    write_ppm_file(ppm, dst, out_file_name);
  }
  printf("Total time spent:%f\n", total_time_spent);
  free(ppm.image);
  free(dst);
  return 0;
}
