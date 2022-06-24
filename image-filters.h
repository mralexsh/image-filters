#ifndef IMAGE_FILTERS_H_
#define IMAGE_FILTERS_H_

#define FILTERS_AMOUNT 6
#define PPM_VERSION_SIZE 80
#define MASK_SIZE 3
#define FILTER_NAME1 1
#define FILTER_NAME2 2
#define FILTER_NAME3 3
#define FILTER_NAME4 4
#define FILTER_NAME5 5
#define FILTER_NAME6 6
#define FILTER_ITERATION_SIZE 9


typedef unsigned char P_SZ;

P_SZ filter1(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
	     P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9);

P_SZ filter2(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
	     P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9);

P_SZ filter3(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
	     P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9);

P_SZ filter4(P_SZ p, P_SZ p_prev, P_SZ p_mask, int current_x);

P_SZ filter5(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4,
	     P_SZ p5, P_SZ p6, P_SZ p7, P_SZ p8, P_SZ p9);

P_SZ filter6(P_SZ p1, P_SZ p2, P_SZ p3, P_SZ p4, P_SZ p5);

int filter_image(int width,
                 int height,
                 P_SZ *src,
                 P_SZ *dst,
                 int pixel_size,
                 int filter);
#endif
