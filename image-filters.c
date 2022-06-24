#include "image-filters.h"

P_SZ clip_color(int color) {
  if (color > 255) return 255;
  if (color < 0) return 0;
  return (P_SZ) color;
}


P_SZ filter1(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
             P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9) {

  return clip_color(p4 + 8 * p1 - p2 - p3 - p5 - p6 - p7 - p8 - p9);
}

P_SZ filter2(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
             P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9) {

  return (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) / FILTER_ITERATION_SIZE;
}

static inline void simple_sort(P_SZ arr[]) {
  for (int j = 0; j< 8; ++j)
    for(int i = 0; i < 8 - j; ++i)
      if (arr[i] > arr[i + 1]) {
        P_SZ t = arr[i];
        arr[i] = arr[i + 1];
        arr[i + 1] = t;	
      }
}

P_SZ filter3(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
             P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9) {

  P_SZ pixels[9] = {p1, p2, p3, p4, p5, p6, p7, p8, p9};
  simple_sort(pixels);  
  return pixels[4];
}

P_SZ filter4(P_SZ p, P_SZ p_prev, P_SZ p_mask, int current_x) {

  if (current_x > MASK_SIZE)
    return clip_color(p + p_prev - p_mask);
  else
    return p;
}

P_SZ filter5(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
             P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9) {
  return clip_color( 9 * p1 - p2 - p3 - p4 - p5 - p6 - p7 - p8 - p9);
}

P_SZ filter6(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4, P_SZ p5) {
  return clip_color(p1 * 4 - p2 - p3 - p4 - p5);

}

//------------------------------------
int filter_image(int width,
                 int height,
                 P_SZ *src,
                 P_SZ *dst,
                 int pixel_size,
                 int filter) {

  const int width_in_bytes = width * pixel_size;
  const int mask_offset = pixel_size * MASK_SIZE;

  for (int y = 1; y < height - 1; ++y) {
    int pixel_offset = width_in_bytes * y;
    int pixel_offset_prev = pixel_offset - width_in_bytes;
    int pixel_offset_next = pixel_offset + width_in_bytes;


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
                                              p_west,
                                              src[pixel_offset + i - mask_offset],
                                              x);
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
                                              p_east,
                                              p_south,
                                              p_west);
          break;
        default:
          dst[pixel_offset + x + i] = p;
        }
      }
    }
  }
  return 0;
}
