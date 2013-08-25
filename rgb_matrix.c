// ================================================================
// RGB_MATRIX.C
// Please see the comments in rgb_matrix.h for information.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-10-18
// ================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "putil.h"
#include "rgb_matrix.h"

// ----------------------------------------------------------------
rgb_matrix_t* allocate_rgb_matrix(int height, int width,
	int r_fill, int g_fill, int b_fill)
{
	rgb_matrix_t* pmatrix;
	int i, j;

	if ((height < 1) || (width < 1)) {
		fprintf(stderr,
		"allocate_rgb_matrix:  dimensions must be positive; got (%d,%d).\n",
			height, width);
		exit(1);
	}

	pmatrix = (rgb_matrix_t *)malloc_or_die(sizeof(rgb_matrix_t));
	pmatrix->height = height;
	pmatrix->width  = width;
	pmatrix->data = (rgb_pixel_t **)malloc_or_die(
		height * sizeof(rgb_pixel_t *));
	for (i = 0; i < height; i++) {
		pmatrix->data[i] = (rgb_pixel_t*)malloc_or_die(
			width * sizeof(rgb_pixel_t));
		for (j = 0; j < width; j++) {
			pmatrix->data[i][j].r = r_fill;
			pmatrix->data[i][j].g = g_fill;
			pmatrix->data[i][j].b = b_fill;
		}
	}

	return pmatrix;
}

// ----------------------------------------------------------------
void free_rgb_matrix(rgb_matrix_t* prgb_matrix)
{
	int i;
	for (i = 0; i < prgb_matrix->height; i++)
		free(prgb_matrix->data[i]);
	free(prgb_matrix->data);
	free(prgb_matrix);
}

// ----------------------------------------------------------------
void write_rgb_matrix_to_ppm(rgb_matrix_t* prgb_matrix, char* ppm_file_name)
{
	FILE * fp = fopen(ppm_file_name, "w");
	int i, j;

	if (fp == 0) {
		fprintf(stderr, "Couldn't open \"%s\" for write.\n",
			ppm_file_name);
		exit(1);
	}


	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", prgb_matrix->width, prgb_matrix->height);
	fprintf(fp, "255\n");

	for (i = 0; i < prgb_matrix->height; i++) {
		for (j = 0; j < prgb_matrix->width; j++) {
			putc(prgb_matrix->data[i][j].r, fp);
			putc(prgb_matrix->data[i][j].g, fp);
			putc(prgb_matrix->data[i][j].b, fp);
		}
	}

	fclose(fp);
}
