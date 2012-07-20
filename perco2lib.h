// ================================================================
// PERCO2LIB.H
//
// These routines are for percolation on the 2D integer lattice with periodic
// boundary conditions.
//
// Please see README.txt for a top-level description.
//
// ================================================================
// STORAGE REPRESENTATIONS
//
// For elegance, I could have implemented a single "lattice_t" C struct.
// However, I chose not to do so.  Perhaps this code will be re-used by other
// mathematicians.  It is my experience that mathematicians without prior
// exposure to C do not readily apprehend structs, but do readily apprehend
// matrices.  Thus, these routines involve the following:
//
// * A finite square lattice has height M rows by width N columns.
// * Periodic boundary conditions are used.
// * MxN array of vertical   bonds between sites.
// * MxN array of horizontal bonds between sites.
// * MxN array of site marks, used for various purposes as described below.
// * Rows    are indexed 0 .. M-1
// * Columns are indexed 0 .. N-1
// * Vertical   bonds are indexed by the site above them.
//   Indices are [0..M-1][0..N-1]
// * Horizontal bonds are indexed by the site left of them.
//   Indices are [0..M-1][0..N-1]
//
// Example with M=4, N=4:
//
// Col 0          Col 3
//   . - . - . - . -       Row 0
//   |   |   |   |
//   . - . - . - . -       Bond marked with * is vbonds[1][2]
//   |   |   *   |         Bond marked with x is hbonds[2][1]
//   . - . x 1 - . -       Site marked with 1 is at    [2][2]
//   |   |   |   |
//   . - . - . - . -       Row 3
//   |   |   |   |
//
// A populated lattice is completely specified by M, N, vbonds[][], and
// hbonds[][].
//
// Each of these routines is tested in perco2.c.  So, please see perco2.c
// for exmaples of how to use these routines.
//
// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
//
// This code is released under the terms of the GNU GPL.  Please see
// COPYING.txt in the same directory as this file.
// ================================================================

#ifndef PERCO2LIB_H
#define PERCO2LIB_H

// ----------------------------------------------------------------
// Two-dimensional percolation.  Leaving this as 'd' rather than hard-coding
// '2' throughout simplified the port to 3D percolation (which I am not
// presenting for my computational exam).
#define d 2

// A macro for comparing points in 2D:
#define pteq(A,B) (((A)[0]==(B)[0])&&((A)[1]==(B)[1]))

// Maximum number of neighbors for a lattice site on the 2D square lattice:
#define MAXNEI 4

// For use by print_lattice() and the cluster-marking routines:
#define SITECHAR    '.'
#define VISITEDCHAR '@'

// Critical bond density for 2D:
#define P_C 0.5

// ----------------------------------------------------------------
// Allocates an M by N matrix of integers, e.g. vbonds[][], hbonds[][], or
// site_marks[][].  The caller should use free_matrix() to release the
// dynamically allocated memory.
int** allocate_matrix(int M, int N, int fill);

// Frees the memory obtained by allocate_matrix().
void free_matrix(int** matrix, int M, int N);

// Sets all elements of the matrix to the same value.  Other ways to populate
// the matrix elements are left to the caller's imagination.
void fill_matrix(int** matrix, int M, int N, int value);

// Populates lattice bonds, IID and open (connected) with probability p.
void populate_bonds(int** vbonds, int** hbonds, int M, int N, double p);

// ----------------------------------------------------------------
// Various routines have one or two distinguished points on the lattice.
// The first is in mid-lattice, i.e. with coordinates (M/2,N/2).
// The second is one position down and to the right.
void set_A1(int A1[d], int M, int N);
void set_A1_A2(int A1[d], int A2[d], int M, int N);

// ----------------------------------------------------------------
// * vbonds, hbonds, M, and N represent the lattice.
// * A1 is the center point (or origin), represented by a d-tuple of integer
//   coordinates.
// * After this routine returns, the reference argument *pnumnei contains
//   the number of nearest-neighbor sites connected to A1 via a horizontal or
//   vertical bond.  The first *pnumnei d-tuples in the caller-provided
//   neighbors[][] array contain the integer coordinates of those neighbors.
// populate_bonds() must have been called first.
void get_bonded_neighbors(int** vbonds, int** hbonds, int M, int N,
	int A1[d], int neighbors[MAXNEI][d], int* pnumnei);

// Similar to get_bonded_neighbors(), but only returns the number of neighbors
// without returning their coordinates.
int  get_num_neighbors(int** vbonds, int** hbonds, int M, int N, int A1[d]);

// * vbonds, hbonds, M, and N represent the lattice.
// * site_marks[][] is a caller-provided MxN workspace.
// * A1 is the center point (or origin).
// The cluster containing the center point is identified.  At each cluster site
// in the site_marks[][] array, the mark_value is written.  All other elements
// of site_marks[][] (i.e. sites in other clusters) are left unmodified.
// populate_bonds() must have been called first.
void mark_one_cluster(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int mark_value);
// Recursive subroutine called by mark_one_cluster().
void mark_one_cluster_aux(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int mark_value);

// * vbonds, hbonds, M, and N represent the lattice.
// * site_marks[][] is a caller-provided MxN workspace.
// * A1 is the center point (or origin).
// The cluster containing the center point is identified.  At each cluster site
// in the site_marks[][] array, the mark_value is written.  All other elements
// of site_marks[][] (i.e. sites in other clusters) are left unmodified.
// populate_bonds() must have been called first.
double get_cluster_size(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d]);

// Calls get_cluster_size() a specified number of times, returning the sample
// mean of the size of the cluster containing point A1.
// populate_bonds() must have been called first.
double get_mean_C0_size(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d]);

// Same as get_mean_C0_size(), but uses the second-largest cluster.
// populate_bonds() must have been called first.
double get_mean_finite_C0_size(
	int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d]);

// * vbonds, hbonds, M, and N represent the lattice.
// * site_marks[][] is a caller-provided MxN workspace.
// * Computational details are described in the comments above get_corrlen()
//   in perco2lib.c.
double get_corrlen(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d]);

// * vbonds, hbonds, M, and N represent the lattice.
// * site_marks[][] is a caller-provided MxN workspace.
// Returns 1 if there is a path from point A1 to point A2, else 0.
// populate_bonds() must have been called first.
int A1_oo_A2(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d]);

// Over a specified number of repetitions (the reps argument), populates
// lattices and finds the fraction of those in which there is a path from point
// A1 to point A2.
double P_A1_oo_A2(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d], int A2[d]);

// * vbonds, hbonds, M, and N represent the lattice.
// * site_marks[][] is a caller-provided MxN workspace.
// * Upon return, the reference argument *pnum_clusters contains the number
//   of clusters in the lattice.
void mark_cluster_numbers(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int* pnum_clusters);

// * vbonds, hbond, M, and N represent the lattice.
// * site_marks[][] must have already been populated by calling
//   mark_cluster_numbers().
// For each point in the lattice, checks to see that that point's cluster
// number is the same as the cluster number of each of its bonded neighbors.
// For example, the coding error (here, with M=2 and N=4)
//
//   1 -- 2    2 -- 2
//   |    |
//   1 -- 1    3    4
//
// will be detected.  However, the coding error (here, with M=2 and N=4)
//
//   1 -- 1    1 -- 1
//   |    |
//   1 -- 1    2    3
//
// will not be detected by this routine.
void sanity_check_cluster_numbers(int** site_marks, int** vbonds, int** hbonds,
	int M, int N);

// * vbonds, hbonds, M, and N represent the lattice.
// * site_marks[][] is a caller-provided MxN workspace.
// * The caller-provided array cluster_sizes[] should be as big as the number
//   of clusters in the lattice.  If this is unknown, the caller may safely
//   pass an integer array of worst-case length M*N.
// * On return, the reference argument *pC_clno contains the index of the
//   largest cluster.
// populate_bonds() and mark_cluster_numbers() must have been called first.
void get_cluster_sizes(int** site_marks, int M, int N, int num_clusters,
	int* cluster_sizes, int* pC_clno);

// * vbonds, hbonds, M, and N represent the lattice.
// * site_marks[][] is a caller-provided MxN workspace.
// Calls mark_cluster_numbers() and get_cluster_sizes().
// Returns 1 if point A is in the largest cluster, else 0.
// populate_bonds() must have been called first.
int A_in_C(int** site_marks, int** vbonds, int** hbonds, int M, int N,
	double p, int A[d], int* cluster_sizes);
// Over a specified number of repetitions (the reps argument), populates
// lattices and finds the fraction of those in which there is a path from point
// A1 to point A2.
double P_A_in_C(int** site_marks, int** vbonds, int** hbonds, int M, int N,
	double p, int reps, int A[d]);

// Returns 1 if point A1 or point A2 is in the largest cluster, else 0.
// populate_bonds() must have been called first.
int A1_or_A2_in_C(int** site_marks, int** vbonds, int** hbonds, int M, int N,
	double p, int A1[d], int A2[d], int* cluster_sizes);
// Over a specified number of repetitions (the reps argument), populates
// lattices and finds the fraction of those in which either point A1 or point
// A2 is in the largest cluster.
double P_A1_or_A2_in_C(int** site_marks,
	int** vbonds, int** hbonds, int M, int N,
	double p, int reps, int A1[d], int A2[d]);

#endif // PERCO2LIB_H
