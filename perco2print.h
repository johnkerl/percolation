// ================================================================
// PERCO2PRINT.H
//
// These are routines for drawing a 2D bond configuration to the screen
// using ASCII art.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
// ================================================================

#ifndef PERCO2PRINT_H
#define PERCO2PRINT_H

#include "perco2lib.h"

// ----------------------------------------------------------------
void print_lattice(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d]);

// Output of "./perco2 print M=8 N=14 p=0.6":
//
// ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
// o                                                         o
// o . - . - .   . - . - .   . - .   . - .   .   .   .   . - o
// o     |   |   |   |   |       |               |           o
// o . - .   . - . - .   . - . - . - .   .   .   .   .   . - o
// o |               |       |   |   |   |   |   |       |   o
// o .   . - .   .   . - .   . - .   .   . - . - . - . - .   o
// o     |   |   |           |   |   |       |   |           o
// o . - . - . - . - .   . - .   .   .   .   . - . - .   . - o
// o         |   |       |   |   |   |   |       |   |       o
// o . - .   . - .   . - . - . - A   .   . - .   .   . - . - o
// o |   |       |   |   |   |   |           |   |           o
// o . - .   . - . - .   . - .   .   B - .   .   .   .   . - o
// o     |       |   |       |       |   |   |   |   |   |   o
// o .   . - . - .   .   .   . - .   . - .   . - .   . - . - o
// o |   |           |   |   |   |                           o
// o .   .   .   .   .   .   . - .   .   . - . - .   .   .   o
// o |           |           |       |   |   |   |       |   o
// o                                                         o
// ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

// ----------------------------------------------------------------
void print_lattice_and_cluster_numbers(int** site_marks,
	int** vbonds, int** hbonds, int M, int N);

// Output of "./perco2 clnos M=8 N=14 p=0.6":
//
// ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
// o                                                         o
// o 0 - 0   0 - 0 - 0 - 0 - 0 - 0   1   0 - 0 - 0 - 0 - 0 - o
// o             |   |   |   |   |       |       |       |   o
// o 0   0   0 - 0   0 - 0 - 0 - 0   0 - 0   2   0 - 0 - 0 - o
// o     |               |       |       |       |   |   |   o
// o 0 - 0 - 0 - 0 - 0   0   3   0   0 - 0 - 0   0   0 - 0 - o
// o |   |   |       |                       |   |   |       o
// o 0 - 0 - 0 - 0   0 - 0 - 0 - 0   4   0   0 - 0   0   0 - o
// o |   |   |   |   |                   |   |   |   |       o
// o 0 - 0   0   0   0 - 0   5   6   0 - 0 - 0   0 - 0 - 0 - o
// o |                   |               |   |   |       |   o
// o 0 - 0   0 - 0 - 0 - 0   0 - 0   0   0 - 0   0   0 - 0   o
// o |   |   |       |   |   |   |   |   |       |   |   |   o
// o 0 - 0 - 0   0 - 0   0   0 - 0 - 0 - 0 - 0   0   0 - 0 - o
// o     |   |       |       |   |       |   |   |       |   o
// o 0 - 0 - 0 - 0 - 0 - 0 - 0 - 0 - 0 - 0 - 0   0   0 - 0 - o
// o     |   |       |   |       |       |   |   |   |   |   o
// ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

#endif // PERCO2PRINT_H
