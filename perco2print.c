// ================================================================
// PERCO2PRINT.C
// Please see the comments in perco2print.h for information.
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
#include "perco2print.h"
#include "rcmrand.h"

// ----------------------------------------------------------------
// * M rows by N columns.
// * Rows    0 .. M-1
// * Columns 0 .. N-1
// * Vertical   bonds are indexed by the site above them.
//   Indices are [0..M-1][0..N-1]
// * Horizontal bonds are indexed by the site left of them.
//   Indices are [0..M-1][0..N-1]
//
// Example with M=4, N=4
//
// Col 0          Col 4
//   . - . - . - . -       Row 0
//   |   |   |   |
//   . - . - . - . -       Bond marked with * is vbonds[1][2]
//   |   |   *   |         Bond marked with x is hbonds[2][1]
//   . - . x 1 - . -       Site marked with 1 is at    [2][2]
//   |   |   |   |
//   . - . - . - . -       Row 3
//   |   |   |   |
//

// Sample output:
// kerl@sprax% perco2 print MN=6
//
// ooooooooooooooooooooooooooo
// o                         o
// o . - . - . - .   . - . - o
// o |       |   |       |   o
// o .   .   .   . - . - .   o
// o         |               o
// o . - .   . - . - .   . - o
// o     |       |           o
// o . - . - . - A - .   . - o
// o |           |           o
// o .   . - . - .   B   .   o
// o     |       |           o
// o .   .   .   . - . - .   o
// o |           |   |   |   o
// ooooooooooooooooooooooooooo

void print_lattice(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d])
{
	int i, j;
	int ij[d];

	for (j = 0; j < N; j++)
		printf("oooo");
	printf("ooo\n");

	printf("o ");
	for (j = 0; j < N; j++)
		printf("    ");
	printf("o\n");

	// For each row
	for (i = 0; i < M; i++) {

		printf("o ");
		// Display the horizontal bonds and the sites.
		// Horizontal bonds are indexed by the site left of them.
		// Indices are [0..M][0..N-1]
		for (j = 0; j < N; j++) {
			ij[0] = i;
			ij[1] = j;

			if (pteq(ij, A1))
				printf("A");
			else if (pteq(ij, A2))
				printf("B");
			else
				printf("%c", site_marks[i][j]);
			if (j < N) {
				if (hbonds[i][j])
					printf(" - ");
				else
					printf("   ");
			}
		}
		printf("o\n");

		// Display the vertical bonds.
		// Vertical bonds are indexed by the site above them.
		// Indices are [0..M-1][0..N]
		if (i < M) {
			printf("o ");
			for (j = 0; j < N; j++) {
				if (vbonds[i][j])
					printf("|");
				else
					printf(" ");
				if (j < N)
					printf("   ");
			}
			printf("o\n");
		}
	}

	for (j = 0; j < N; j++)
		printf("oooo");
	printf("ooo\n");
}

// ----------------------------------------------------------------
// Sample output:
// kerl@sprax% perco2 clnos MN=6 p=.4
//
// ooooooooooooooooooooooooooo
// o                         o
// o .   . - .   . - . - . - o
// o |                       o
// o .   .   .   .   .   . - o
// o |               |   |   o
// o . - .   . - . - . - .   o
// o |                       o
// o .   . - .   .   .   .   o
// o             |   |       o
// o .   . - .   .   .   . - o
// o             |   |       o
// o . - . - .   .   .   .   o
// o         |       |       o
// ooooooooooooooooooooooooooo
//
// ooooooooooooooooooooooooooo
// o                         o
// o 0   1 - 1   0 - 0 - 0 - o
// o |                       o
// o 0   2   3   4   0   0 - o
// o |               |   |   o
// o 0 - 0   0 - 0 - 0 - 0   o
// o |                       o
// o 0   5 - 5   6   0   7   o
// o             |   |       o
// o 8   9 - 9   6   0   8 - o
// o             |   |       o
// o 1 - 1 - 1   6   0   10   o
// o         |       |       o
// ooooooooooooooooooooooooooo

void print_lattice_and_cluster_numbers(int** site_marks,
	int** vbonds, int** hbonds, int M, int N)
{
	int i, j;
	int ij[d];

	for (j = 0; j < N; j++)
		printf("oooo");
	printf("ooo\n");

	printf("o ");
	for (j = 0; j < N; j++)
		printf("    ");
	printf("o\n");

	// For each row
	for (i = 0; i < M; i++) {

		printf("o ");
		// Display the horizontal bonds and the sites.
		// Horizontal bonds are indexed by the site left of them.
		// Indices are [0..M][0..N-1]
		for (j = 0; j < N; j++) {
			ij[0] = i;
			ij[1] = j;

			printf("%d", site_marks[i][j]);
			if (j < N) {
				if (hbonds[i][j])
					printf(" - ");
				else
					printf("   ");
			}
		}
		printf("o\n");

		// Display the vertical bonds.
		// Vertical bonds are indexed by the site above them.
		// Indices are [0..M-1][0..N]
		if (i < M) {
			printf("o ");
			for (j = 0; j < N; j++) {
				if (vbonds[i][j])
					printf("|");
				else
					printf(" ");
				if (j < N)
					printf("   ");
			}
			printf("o\n");
		}
	}

	for (j = 0; j < N; j++)
		printf("oooo");
	printf("ooo\n");
}
