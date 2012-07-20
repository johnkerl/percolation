// ================================================================
// PERCO2PLOT.C
// Please see the comments in perco2plot.h for information.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
//
// This code is released under the terms of the GNU GPL.  Please see
// COPYING.txt in the same directory as this file.
// ================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "putil.h"
#include "perco2lib.h"
#include "rgb_matrix.h"
#include "rcmrand.h"

#define WHITE_R 0xff
#define WHITE_G 0xff
#define WHITE_B 0xff

#define GREY_R  0x80
#define GREY_G  0x80
#define GREY_B  0x80

#define BLACK_R 0x00
#define BLACK_G 0x00
#define BLACK_B 0x00

#define RED_R   0xff
#define RED_G   0x00
#define RED_B   0x00

#define BLUE_R  0x00
#define BLUE_G  0x00
#define BLUE_B  0xff

static void plot_site(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b);
static void maybe_plot_site(rgb_matrix_t* pixels, int i, int j,
	int M, int N, int r, int g, int b);
static void plot_hbond(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b);
static void plot_vbond(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b);
void plot_gap(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b);
static void plot_block(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b);

// ----------------------------------------------------------------
// Write a PPM file.  Use 3x3 pixels for each site and two bonds:

// o - -
// | . .
// | . .

// o - - o - - o . . o
// | . . . . . | . . .
// | . . . . . | . . .
// o . . o - - o . . o
// . . . | . . . . . .
// . . . | . . . . . .
// o - - o - - o - - o
// . . . | . . . . . .
// . . . | . . . . . .
// o . . o . . o . . o

// Fill the background in white.
// Plot sites in blue.
// Plot A1, A2 in red.
// Plot bonds in black.

void plot_lattice(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d], char* image_file_name)
{
	int i, j;
	int pixel_height = 3*M+1;
	int pixel_width  = 3*N+1;
	rgb_matrix_t* pixels = allocate_rgb_matrix(pixel_height, pixel_width,
		WHITE_R, WHITE_G, WHITE_B);

	// Plot sites in blue.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			plot_site(pixels, i, j, BLUE_R, BLUE_G, BLUE_B);

	// Plot A1, A2 in red.
	maybe_plot_site(pixels, A1[0], A1[1], M, N, RED_R, RED_G, RED_B);
	maybe_plot_site(pixels, A2[0], A2[1], M, N, RED_R, RED_G, RED_B);

	// Plot bonds in black.
	// Horizontal bonds are indexed by the site left of them.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (hbonds[i][j])
				plot_hbond(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	// Plot bonds in black.
	// Vertical bonds are indexed by the site above them.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (vbonds[i][j])
				plot_vbond(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	write_rgb_matrix_to_ppm(pixels, image_file_name);
	free_rgb_matrix(pixels);
}

// ----------------------------------------------------------------
void plot_lattice_and_one_cluster(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d], char* image_file_name)
{
	int i, j;
	int pixel_height = 3*M+1;
	int pixel_width  = 3*N+1;
	rgb_matrix_t* pixels = allocate_rgb_matrix(pixel_height, pixel_width,
		WHITE_R, WHITE_G, WHITE_B);

	// Plot the cluster.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (site_marks[i][j] == VISITEDCHAR)
				plot_block(pixels, i, j, BLUE_R, BLUE_G, BLUE_B);

	// Plot bonds in black.
	// Horizontal bonds are indexed by the site left of them.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (hbonds[i][j])
				plot_hbond(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	// Plot bonds in black.
	// Vertical bonds are indexed by the site above them.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (vbonds[i][j])
				plot_vbond(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	// Plot sites.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			plot_site(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	write_rgb_matrix_to_ppm(pixels, image_file_name);
	free_rgb_matrix(pixels);
}

// ----------------------------------------------------------------
void plot_one_cluster_compactly(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d], char* image_file_name)
{
	int i, j;
	int pixel_height = M+1;
	int pixel_width  = N+1;
	rgb_matrix_t* pixels = allocate_rgb_matrix(pixel_height, pixel_width,
		WHITE_R, WHITE_G, WHITE_B);

	// Plot the clusters.
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			if (site_marks[i][j] == VISITEDCHAR) {
				pixels->data[i][j].r = BLUE_R;
				pixels->data[i][j].g = BLUE_G;
				pixels->data[i][j].b = BLUE_B;
			}
		}
	}

	write_rgb_matrix_to_ppm(pixels, image_file_name);
	free_rgb_matrix(pixels);
}

// ----------------------------------------------------------------
void plot_lattice_and_all_clusters(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d], char* image_file_name)
{
	int i, j;
	int pixel_height = 3*M+1;
	int pixel_width  = 3*N+1;
	rgb_matrix_t* pixels = allocate_rgb_matrix(pixel_height, pixel_width,
		WHITE_R, WHITE_G, WHITE_B);
	int max_clno = 0;
	int* reds;
	int* greens;
	int* blues;

	// Find the number of clusters.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (site_marks[i][j] > max_clno)
				max_clno = site_marks[i][j];
	reds   = (int *)malloc_or_die((max_clno+1)*sizeof(int));
	greens = (int *)malloc_or_die((max_clno+1)*sizeof(int));
	blues  = (int *)malloc_or_die((max_clno+1)*sizeof(int));

	for (i = 1; i <= max_clno; i++) {
#if 0
		reds  [i] = IMODRANDOM(255);
		greens[i] = IMODRANDOM(255);
		blues [i] = IMODRANDOM(255);
#else
		reds  [i] = (unsigned char)RANDRANGE(32,224);
		greens[i] = (unsigned char)RANDRANGE(32,224);
		blues [i] = (unsigned char)RANDRANGE(32,224);
#endif
	}

	// Plot the clusters.
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			if (site_marks[i][j] == 0) {
				plot_block(pixels, i, j, GREY_R, GREY_G, GREY_B);
			}
			else {
				int clno = site_marks[i][j];
				plot_block(pixels, i, j, reds[clno], greens[clno], blues[clno]);
			}
		}
	}

	// Plot bonds in black.
	// Horizontal bonds are indexed by the site left of them.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (hbonds[i][j])
				plot_hbond(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	// Plot bonds in black.
	// Vertical bonds are indexed by the site above them.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (vbonds[i][j])
				plot_vbond(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	// Plot sites.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			plot_site(pixels, i, j, BLACK_R, BLACK_G, BLACK_B);

	write_rgb_matrix_to_ppm(pixels, image_file_name);
	free_rgb_matrix(pixels);
	free(reds);
	free(greens);
	free(blues);
}

// ----------------------------------------------------------------
void plot_all_clusters_compactly(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d], char* image_file_name)
{
	int i, j;
	int pixel_height = M+1;
	int pixel_width  = N+1;
	rgb_matrix_t* pixels = allocate_rgb_matrix(pixel_height, pixel_width,
		WHITE_R, WHITE_G, WHITE_B);
	int max_clno = 0;
	int* reds;
	int* greens;
	int* blues;

	// Find the number of clusters.
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (site_marks[i][j] > max_clno)
				max_clno = site_marks[i][j];
	reds   = (int *)malloc_or_die((max_clno+1)*sizeof(int));
	greens = (int *)malloc_or_die((max_clno+1)*sizeof(int));
	blues  = (int *)malloc_or_die((max_clno+1)*sizeof(int));

	for (i = 1; i <= max_clno; i++) {
#if 1
		reds  [i] = IMODRANDOM(255);
		greens[i] = IMODRANDOM(255);
		blues [i] = IMODRANDOM(255);
#else
		reds  [i] = (unsigned char)RANDRANGE(32,224);
		greens[i] = (unsigned char)RANDRANGE(32,224);
		blues [i] = (unsigned char)RANDRANGE(32,224);
#endif
	}

	// Plot the clusters.
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			if (site_marks[i][j] == 0) {
				pixels->data[i][j].r = GREY_R;
				pixels->data[i][j].g = GREY_G;
				pixels->data[i][j].b = GREY_B;
			}
			else {
				int clno = site_marks[i][j];
				pixels->data[i][j].r = reds  [clno];
				pixels->data[i][j].g = greens[clno];
				pixels->data[i][j].b = blues [clno];
			}
		}
	}

	write_rgb_matrix_to_ppm(pixels, image_file_name);
	free_rgb_matrix(pixels);
	free(reds);
	free(greens);
	free(blues);
}

// ----------------------------------------------------------------
static void plot_site(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b)
{
	pixels->data[3*i][3*j].r = r;
	pixels->data[3*i][3*j].g = g;
	pixels->data[3*i][3*j].b = b;
}

// ----------------------------------------------------------------
static void maybe_plot_site(rgb_matrix_t* pixels, int i, int j,
	int M, int N, int r, int g, int b)
{
	if ((0 <= i) && (i < M)) {
		if ((0 <= j) && (j < M)) {
			pixels->data[3*i][3*j].r = r;
			pixels->data[3*i][3*j].g = g;
			pixels->data[3*i][3*j].b = b;
		}
	}
}

// ----------------------------------------------------------------
// Horizontal bonds are indexed by the site left of them.
static void plot_hbond(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b)
{
	int ti = 3*i;
	int tj = 3*j;
	int v;

	for (v = 1; v <= 2; v++) {
		pixels->data[ti][tj+v].r = r;
		pixels->data[ti][tj+v].g = g;
		pixels->data[ti][tj+v].b = b;
	}
}

// ----------------------------------------------------------------
// Vertical bonds are indexed by the site above them.
static void plot_vbond(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b)
{
	int ti = 3*i;
	int tj = 3*j;
	int u;

	for (u = 1; u <= 2; u++) {
		pixels->data[ti+u][tj].r = r;
		pixels->data[ti+u][tj].g = g;
		pixels->data[ti+u][tj].b = b;
	}
}

// ----------------------------------------------------------------
void plot_gap(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b)
{
	int ti = 3*i;
	int tj = 3*j;
	int u, v;

	for (u = 1; u <= 2; u++) {
		for (v = 1; v <= 2; v++) {
			pixels->data[ti+u][tj+v].r = r;
			pixels->data[ti+u][tj+v].g = g;
			pixels->data[ti+u][tj+v].b = b;
		}
	}
}

// ----------------------------------------------------------------
static void plot_block(rgb_matrix_t* pixels, int i, int j,
	int r, int g, int b)
{
	int ti = 3*i;
	int tj = 3*j;
	int u, v;

	for (u = 0; u <= 2; u++) {
		for (v = 0; v <= 2; v++) {
			pixels->data[ti+u][tj+v].r = r;
			pixels->data[ti+u][tj+v].g = g;
			pixels->data[ti+u][tj+v].b = b;
		}
	}
}
