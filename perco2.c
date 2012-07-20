// ================================================================
// PERCO2.C
//
// This program calls the routines found in perco2lib.h and perco2lib.c.  The
// goals of the research project are as described in the comments at the top of
// perco2lib.h.  In this program, as I was writing perco2lib, I incrementally
// implemented routines which test the perco2lib routines:  I made sure a
// simple routine (e.g. print_lattice()) was correct before writing the next
// one.  Debugging a complete software suite is like finding a needle in a
// haystack; I circumvent that problem by writing and testing routines one at a
// time, ensuring that I build up a needle-free haystack one needle-free
// handful at a time.  Thus, the routines in this file appear in increasing
// order of complexity.  The three routines marked in main(), below, address
// the research question as described in frgslides.pdf and perco2lib.h.
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
#include "perco2plot.h"
#include "rcmrand.h"

// ----------------------------------------------------------------
// Prototypes for functions local to this file:
static void main_usage(char* argv0);
static void usage(char* argv0, char* argv1, int print_reps_usage);

static void test_print_lattice        (int argc, char** argv);
static void test_plot_lattice         (int argc, char** argv);
static void test_get_bonded_neighbors (int argc, char** argv);
static void test_cluster              (int argc, char** argv);
static void test_plot_cluster         (int argc, char** argv);
static void test_mean_C0_size         (int argc, char** argv);
static void test_mean_finite_C0_size  (int argc, char** argv);
static void test_corrlen              (int argc, char** argv);
static void test_A1_oo_A2             (int argc, char** argv);
static void test_P_A1_oo_A2           (int argc, char** argv);
static void test_cluster_numbers      (int argc, char** argv);
static void test_plot_clusters        (int argc, char** argv);
static void test_cluster_sizes        (int argc, char** argv);
static void test_A_in_C               (int argc, char** argv);
static void test_P_A_in_C             (int argc, char** argv);
static void test_A1_or_A2_in_C        (int argc, char** argv);
static void test_P_A1_or_A2_in_C      (int argc, char** argv);

// ----------------------------------------------------------------
int main(int argc, char** argv)
{
	STRANDOM(); // Seed the random-number generator.

	// If the user invoked us with no arguments, give them a usage message.

	if (argc < 2)
		main_usage(argv[0]);

	// Invoke the appropriate subroutine (if any) for the first argument the
	// user typed, passing all remaining arguments along to that subroutine.
	// If the first argument is unrecognized, give them a usage message.
	//
	// Sample command line:  "./perco2 print p=0.6 M=8 N=16".

	if      (strcmp(argv[1], "print") == 0)
		test_print_lattice(argc, argv);
	else if (strcmp(argv[1], "plot") == 0)
		test_plot_lattice(argc, argv);

	else if (strcmp(argv[1], "nei") == 0)
		test_get_bonded_neighbors(argc, argv);

	else if (strcmp(argv[1], "cluster") == 0)
		test_cluster(argc, argv);
	else if (strcmp(argv[1], "plotcluster") == 0)
		test_plot_cluster(argc, argv);
	else if (strcmp(argv[1], "meanC0size") == 0)
		test_mean_C0_size(argc, argv);
	else if (strcmp(argv[1], "meanfC0size") == 0)
		test_mean_finite_C0_size(argc, argv);
	else if (strcmp(argv[1], "corrlen") == 0)
		test_corrlen(argc, argv);

	else if (strcmp(argv[1], "1o2") == 0)
		test_A1_oo_A2(argc, argv);
	else if (strcmp(argv[1], "P1o2") == 0) // This is tau(p) in greeks.sh.
		test_P_A1_oo_A2(argc, argv);

	else if (strcmp(argv[1], "clnos") == 0)
		test_cluster_numbers(argc, argv);
	else if (strcmp(argv[1], "plotclusters") == 0)
		test_plot_clusters(argc, argv);
	else if (strcmp(argv[1], "clszs") == 0)
		test_cluster_sizes(argc, argv);

	else if (strcmp(argv[1], "AinC") == 0)
		test_A_in_C(argc, argv);
	else if (strcmp(argv[1], "PAinC") == 0) // This is theta(p) in greeks.sh.
		test_P_A_in_C(argc, argv);

	else if (strcmp(argv[1], "U2inC") == 0)
		test_A1_or_A2_in_C(argc, argv);
	else if (strcmp(argv[1], "PU2inC") == 0) // This is sigma(p) in greeks.sh.
		test_P_A1_or_A2_in_C(argc, argv);

	else
		main_usage(argv[0]);

	return 0;
}

// ----------------------------------------------------------------
// Usage routine invoked from main() when the user doesn't supply argument 1,
// or supplies an unrecognized one.
static void main_usage(char* argv0)
{
	fprintf(stderr, "Usage: %s {command} [options].\n", argv0);
	fprintf(stderr, "Commands: print plot nei cluster plotcluster meanC0size "
		"meanfC0size corrlen\n");
	fprintf(stderr, "  1o2 P1o2 clnos plotclusters clszs\n");
	fprintf(stderr, "  AinC PAinC U2inC PU2inC\n");
	exit(1);
}

// ----------------------------------------------------------------
// Usage routine invoked by individual command handlers in the case of invalid
// argument 2 and above.  All of those routines take mostly the same syntax, so
// they can all use the same usage routine.  An exception is that for some of
// them a "reps=10000" argument is valid, while for others of them it is not.
static void usage(char* argv0, char* argv1, int print_reps_usage)
{
	fprintf(stderr, "Usage: %s %s [options]\n", argv0, argv1);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "M=[...]    : Lattice height (M >= 3)\n");
	fprintf(stderr, "N=[...]    : Lattice width  (N >= 3)\n");
	fprintf(stderr, "p=[...]    : Bond probability (0 <= p <= 1)\n");
	if (print_reps_usage)
		fprintf(stderr, "reps=[...] : Number of repetitions for P.\n");
	exit(1);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds and plots it to the screen using ASCII art.
static void test_print_lattice(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];

	// Parse the command line.
	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	// Validate parameters.
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	// Allocate storage for the lattice.
	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);

	// Pick center points of the lattice.
	set_A1_A2(A1, A2, M, N);

	// Populate the bonds with probability p.
	populate_bonds(vbonds, hbonds, M, N, p);

	// Print the lattice.
	print_lattice(site_marks, vbonds, hbonds, M, N, A1, A2);

	// Free the lattice storage.
	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds and writes it to a PPM file.
static void test_plot_lattice(int argc, char** argv)
{
	int   M = 200;
	int   N = 200;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	char* image_file_name = "p2.ppm";

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (strncmp(argv[argi], "f=", 2) == 0)
			image_file_name = &argv[argi][2];
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	populate_bonds(vbonds, hbonds, M, N, p);
	plot_lattice(site_marks, vbonds, hbonds, M, N, A1, A2,
		image_file_name);
	printf("Wrote %s.\n", image_file_name);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds, prints the lattice, and lists the bonded
// neighbors of the center site.  One may then visually verify the
// identification of neighbors.
static void test_get_bonded_neighbors(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	int neighbors[MAXNEI][d];
	int numnei;
	int k;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	populate_bonds(vbonds, hbonds, M, N, p);
	print_lattice(site_marks, vbonds, hbonds, M, N, A1, A2);
	printf("\n");

	get_bonded_neighbors(vbonds, hbonds, M, N, A1, neighbors, &numnei);

	printf("Bonded neighbors of A1 at (%d, %d):\n", A1[0], A1[1]);

	if (numnei == 0)
		printf("(none)\n");

	for (k = 0; k < numnei; k++) {
		printf("  ( %3d, %3d )\n", neighbors[k][0], neighbors[k][1]);
	}

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds, marks the cluster containing the center
// site, and prints the results as ASCII art.  One may then visually verify
// the cluster-marking computation.
static void test_cluster(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	populate_bonds(vbonds, hbonds, M, N, p);
	mark_one_cluster(site_marks, vbonds, hbonds, M, N, A1, VISITEDCHAR);
	print_lattice(site_marks, vbonds, hbonds, M, N, A1, A2);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds, marks the cluster containing the center
// site, and plots the results as a PPM file.  One may then visually verify
// the cluster-marking computation.
static void test_plot_cluster(int argc, char** argv)
{
	int   M = 200;
	int   N = 200;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	char* image_file_name = "p2.ppm";
	int compact = 0;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (strncmp(argv[argi], "f=", 2) == 0)
			image_file_name = &argv[argi][2];
		else if (sscanf(argv[argi], "c=%d", &compact) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	populate_bonds(vbonds, hbonds, M, N, p);
	mark_one_cluster(site_marks, vbonds, hbonds, M, N, A1, VISITEDCHAR);
	if (compact)
		plot_one_cluster_compactly(site_marks, vbonds, hbonds, M, N,
			A1, A2, image_file_name);
	else
		plot_lattice_and_one_cluster(site_marks, vbonds, hbonds, M, N,
			A1, A2, image_file_name);
	printf("Wrote %s.\n", image_file_name);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Over a specified number of repetitions: randomly populates lattice bonds and
// computes the size of the largest cluster.  Averages those over the number of
// lattice instantiations.
static void test_mean_C0_size(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int   reps = 1000;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	double mean_C0_size;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "reps=%d", &reps) == 1)
			;
		else
			usage(argv[0], argv[1], 1);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	mean_C0_size = get_mean_C0_size(site_marks, vbonds, hbonds,
		M, N, p, reps, A1);
	printf("M=%d N=%d p=%.4lf reps=%d <size>=%11.7lf <density>=%11.7lf\n",
		M, N, p, reps, mean_C0_size, mean_C0_size/M/N);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Over a specified number of repetitions: randomly populates lattice bonds,
// and (for p < p_c) computes the size of the largest cluster or (for p > p_c)
// computes the size of the second-largest cluster.  Averages those over the
// number of lattice instantiations.
static void test_mean_finite_C0_size(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int   reps = 1000;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	double mean_finite_C0_size;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "reps=%d", &reps) == 1)
			;
		else
			usage(argv[0], argv[1], 1);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	mean_finite_C0_size = get_mean_finite_C0_size(
		site_marks, vbonds, hbonds, M, N, p, reps, A1);
	printf("M=%d N=%d p=%.4lf reps=%d <size>=%11.7lf <density>=%11.7lf\n",
		M, N, p, reps, mean_finite_C0_size,
		mean_finite_C0_size/M/N);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds and computes the correlation length.
static void test_corrlen(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int   reps = 1000;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	double corrlen;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "reps=%d", &reps) == 1)
			;
		else
			usage(argv[0], argv[1], 1);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	corrlen = get_corrlen(site_marks, vbonds, hbonds, M, N, p, reps, A1);
	printf("M=%d N=%d p=%.4lf reps=%d corrlen=%11.7lf\n",
		M, N, p, reps, corrlen);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds and determines if there exists a path from
// the center site to the site diagonally below and to the right.  One may then
// visually verify the path-detection computation.
static void test_A1_oo_A2(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	int ctd;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	populate_bonds(vbonds, hbonds, M, N, p);
	print_lattice(site_marks, vbonds, hbonds, M, N, A1, A2);
	printf("\n");
	ctd = A1_oo_A2(site_marks, vbonds, hbonds, M, N, A1, A2);
	print_lattice(site_marks, vbonds, hbonds, M, N, A1, A2);
	if (ctd)
		printf("Yes\n");
	else
		printf("No\n");

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// For given lattice dimensions and bond density, estimates the probability
// that there exists a path from the center site to the site diagonally below
// and to the right.
static void test_P_A1_oo_A2(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int   reps = 1000;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d];
	int A2[d];
	double P;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "reps=%d", &reps) == 1)
			;
		else
			usage(argv[0], argv[1], 1);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	P = P_A1_oo_A2(site_marks, vbonds, hbonds, M, N, p, reps, A1, A2);
	printf("M=%d N=%d p=%.4lf reps=%d PA1ooA2=%11.7lf\n", M, N, p, reps, P);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds and marks all clusters, plotting the
// results to the screen.  One may then visually verify the path-detection
// computation.
static void test_cluster_numbers(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d] = {-1, -1}; // Not used here
	int A2[d] = {-1, -1}; // Not used here

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);

	populate_bonds(vbonds, hbonds, M, N, p);
	print_lattice(site_marks, vbonds, hbonds, M, N, A1, A2);
	printf("\n");

	mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, 0);
	print_lattice_and_cluster_numbers(site_marks, vbonds, hbonds, M, N);
	sanity_check_cluster_numbers(site_marks, vbonds, hbonds, M, N);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds and marks all clusters, plotting the
// results to a PPM file.
static void test_plot_clusters(int argc, char** argv)
{
	int   M = 200;
	int   N = 200;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d] = {-1, -1}; // Not used here
	int A2[d] = {-1, -1}; // Not used here
	char* image_file_name = "p2.ppm";
	int compact = 1;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (strncmp(argv[argi], "f=", 2) == 0)
			image_file_name = &argv[argi][2];
		else if (sscanf(argv[argi], "c=%d", &compact) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	populate_bonds(vbonds, hbonds, M, N, p);
	mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, 0);
	if (compact)
		plot_all_clusters_compactly(site_marks, vbonds, hbonds, M, N,
			A1, A2, image_file_name);
	else
		plot_lattice_and_all_clusters(site_marks, vbonds, hbonds, M, N,
			A1, A2, image_file_name);
	printf("Wrote %s.\n", image_file_name);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds, marks all clusters, and computes cluster
// sizes.  One may then visually verify the cluster-size computation.
static void test_cluster_sizes(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int print_lattice = 0;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int num_clusters;
	int* cluster_sizes;
	int C_clno; // Number of largest cluster
	int k;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "pl=%d", &print_lattice) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);

	populate_bonds(vbonds, hbonds, M, N, p);
	mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, &num_clusters);
	if (print_lattice)
		print_lattice_and_cluster_numbers(site_marks, vbonds, hbonds, M, N);
	sanity_check_cluster_numbers(site_marks, vbonds, hbonds, M, N);
	cluster_sizes = (int*)malloc_or_die(sizeof(int) * num_clusters);
	get_cluster_sizes(site_marks, M, N, num_clusters, cluster_sizes,
		&C_clno);
	for (k = 0; k < num_clusters; k++) {
		printf("Cluster    %3d", k);
		printf(" size %6d %7.4lf", cluster_sizes[k],
			(double)cluster_sizes[k]/(double)M/(double)N);
		if (k == C_clno)
			printf(" *");
		printf("\n");
	}

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
	free(cluster_sizes);
}

// ----------------------------------------------------------------
// Randomly populates lattice bonds, marks all clusters, and determines whether
// the center site is in the largest cluster.  Plots the result to the screen.
// One may then visually verify the membership computation.
static void test_A_in_C(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int print_lattice = 0;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int num_clusters;
	int* cluster_sizes;
	int C_clno; // Number of largest cluster
	int A[d], A_clno;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "pl=%d", &print_lattice) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1(A, M, N);

	populate_bonds(vbonds, hbonds, M, N, p);
	mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, &num_clusters);
	if (print_lattice)
		print_lattice_and_cluster_numbers(site_marks, vbonds, hbonds, M, N);
	sanity_check_cluster_numbers(site_marks, vbonds, hbonds, M, N);
	cluster_sizes = (int*)malloc_or_die(sizeof(int) * num_clusters);
	get_cluster_sizes(site_marks, M, N, num_clusters, cluster_sizes,
		&C_clno);

	A_clno = site_marks[A[0]][A[1]];
	if (print_lattice) {
		printf("A at %d, %d\n", A[0], A[1]);
		printf("A cluster number = %d\n", A_clno);
		printf("Largest cluster  = %d\n", C_clno);
	}
	printf("%s\n", (A_clno == C_clno) ? "Yes" : "No");

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
	free(cluster_sizes);
}

// ----------------------------------------------------------------
// For given lattice dimensions and bond density, estimates the probability
// that the center site is in the largest cluster.
static void test_P_A_in_C(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A[d];
	int   reps = 1000;
	double P;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "reps=%d", &reps) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1(A, M, N);

	P = P_A_in_C(site_marks, vbonds, hbonds, M, N, p, reps, A);
	printf("M=%d N=%d p=%.4lf reps=%d PAinC=%11.7lf\n", M, N, p, reps, P);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}

// ----------------------------------------------------------------
// For given lattice dimensions and bond density, determinates whether either
// the center site or its diagonal-below-right neighbor is in the largest
// cluster.  Plots the lattice and results to the screen so that the
// computation may be visually verified.
static void test_A1_or_A2_in_C(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int print_lattice = 0;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int num_clusters;
	int* cluster_sizes;
	int C_clno; // Number of largest cluster
	int A1[d], A1_clno;
	int A2[d], A2_clno;
	int yes = 0;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "pl=%d", &print_lattice) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	populate_bonds(vbonds, hbonds, M, N, p);
	mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, &num_clusters);
	if (print_lattice)
		print_lattice_and_cluster_numbers(site_marks, vbonds, hbonds, M, N);
	sanity_check_cluster_numbers(site_marks, vbonds, hbonds, M, N);
	cluster_sizes = (int*)malloc_or_die(sizeof(int) * num_clusters);
	get_cluster_sizes(site_marks, M, N, num_clusters, cluster_sizes,
		&C_clno);

	A1_clno = site_marks[A1[0]][A1[1]];
	A2_clno = site_marks[A2[0]][A2[1]];
	if (print_lattice) {
		printf("A1 at %d, %d\n", A1[0], A1[1]);
		printf("A2 at %d, %d\n", A2[0], A2[1]);
		printf("A1 cluster number = %d\n", A1_clno);
		printf("A2 cluster number = %d\n", A2_clno);
		printf("Largest cluster  = %d\n", C_clno);
	}

	yes = 0;
	if ((A1_clno == C_clno) || (A2_clno == C_clno))
		yes = 1;
	printf("%s\n", yes ? "Yes" : "No");

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
	free(cluster_sizes);
}

// ----------------------------------------------------------------
// For given lattice dimensions and bond density, estimates the probability
// that either the center site or its diagonal-below-right neighbor is in the
// largest cluster.
static void test_P_A1_or_A2_in_C(int argc, char** argv)
{
	int   M = 18;
	int   N = 18;
	double p = 0.6;
	int argi;
	int** vbonds;
	int** hbonds;
	int** site_marks;
	int A1[d], A2[d];
	int   reps = 1000;
	double P;

	for (argi = 2; argi < argc; argi++) {
		if (sscanf(argv[argi], "M=%d", &M) == 1)
			;
		else if (sscanf(argv[argi], "N=%d", &N) == 1)
			;
		else if (sscanf(argv[argi], "MN=%d", &M) == 1)
			N = M;
		else if (sscanf(argv[argi], "p=%lf", &p) == 1)
			;
		else if (sscanf(argv[argi], "reps=%d", &reps) == 1)
			;
		else
			usage(argv[0], argv[1], 0);
	}
	if ((M < 3) || (N < 3))
		usage(argv[0], argv[1], 1);

	vbonds = allocate_matrix(M, N, 0);
	hbonds = allocate_matrix(M, N, 0);
	site_marks = allocate_matrix(M, N, SITECHAR);
	set_A1_A2(A1, A2, M, N);

	P = P_A1_or_A2_in_C(site_marks, vbonds, hbonds, M, N, p, reps, A1, A2);
	printf("M=%d N=%d p=%.4lf reps=%d PU2inC=%11.7lf\n", M, N, p, reps, P);

	free_matrix(vbonds,     M, N);
	free_matrix(hbonds,     M, N);
	free_matrix(site_marks, M, N);
}
