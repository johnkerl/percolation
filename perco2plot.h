// ================================================================
// PERCO2PLOT.H
//
// These are routines for drawing a 2D bond configuration to a PPM-format file.
// I chose PPM format because it is very easy to code up.  One may use
// off-the-shelf image-manipulation software, e.g. the GIMP, to convert to GIF,
// JPEG, encapsulated PostScript, etc. etc.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
// ================================================================

#ifndef PERCO2PLOT_H
#define PERCO2PLOT_H

#include "perco2lib.h"

void plot_lattice(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d],
	char* image_file_name);

void plot_lattice_and_one_cluster(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d],
	char* image_file_name);

void plot_one_cluster_compactly(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d],
	char* image_file_name);

void plot_lattice_and_all_clusters(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d],
	char* image_file_name);

void plot_all_clusters_compactly(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d],
	char* image_file_name);

#endif // PERCO2PLOT_H
