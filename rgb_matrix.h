// ================================================================
// RGB_MATRIX.H
//
// These are routines for dynamically allocating, freeing, and writing to
// a PPM file a matrix of red-green-blue triples.  Filling out the contents
// of the matrix is left to the imagination of the caller.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-10-18
//
// This code is released under the terms of the GNU GPL.  Please see
// COPYING.txt in the same directory as this file.
// ================================================================

#ifndef RGB_MATRIX_H
#define RGB_MATRIX_H

// ----------------------------------------------------------------
typedef struct _rgb_pixel_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb_pixel_t;

typedef struct _rgb_matrix_t {
	int height;
	int width;
	rgb_pixel_t** data;
} rgb_matrix_t;

// ----------------------------------------------------------------
rgb_matrix_t* allocate_rgb_matrix(int height, int width,
	int r_fill, int g_fill, int b_fill);
void free_rgb_matrix(rgb_matrix_t* prgb_matrix);
void write_rgb_matrix_to_ppm(rgb_matrix_t* prgb_matrix, char* ppm_file_name);

#endif // RGB_MATRIX_H
