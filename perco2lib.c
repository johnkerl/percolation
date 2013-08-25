// ================================================================
// PERCO2LIB.C
// Please see the comments in perco2lib.h for information.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
// ================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "putil.h"
#include "perco2lib.h"
#include "rcmrand.h"
#include "perco2print.h"

// ----------------------------------------------------------------
int** allocate_matrix(int M, int N, int fill)
{
	int i, j;
	int** matrix = malloc_or_die(M * sizeof(int*));
	for (i = 0; i < M; i++)
		matrix[i] = malloc_or_die(N * sizeof(int));
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			matrix[i][j] = fill;
	return matrix;
}

// ----------------------------------------------------------------
void free_matrix(int** matrix, int M, int N)
{
	int i;
	for (i = 0; i < M; i++)
		free(matrix[i]);
	free(matrix);
}

// ----------------------------------------------------------------
void fill_matrix(int** matrix, int M, int N, int value)
{
	int i, j;
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			matrix[i][j] = value;
}

// ----------------------------------------------------------------
void populate_bonds(int** vbonds, int** hbonds, int M, int N, double p)
{
	int i, j;
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			vbonds[i][j] = (URANDOM() < p) ? 1 : 0;
			hbonds[i][j] = (URANDOM() < p) ? 1 : 0;
		}
	}
}

// ----------------------------------------------------------------
void set_A1(int A1[d], int M, int N)
{
	A1[0] = M/2;
	A1[1] = N/2;
}

// ----------------------------------------------------------------
void set_A1_A2(int A1[d], int A2[d], int M, int N)
{
	A1[0] = M/2;
	A1[1] = N/2;
	A2[0] = M/2+1;
	A2[1] = N/2+1;
}

// ----------------------------------------------------------------
//     Col j=0        Col j=3
//
//            --j-->
//
//      . - . - . - . -       Row i=0
//      |   |   |   |
//  |   . - . - . - . -       Bond marked with * is vbonds[1][2]
//  |   |   |   *   |         Bond marked with x is hbonds[2][1]
//  i   . - . x 1 - . -       Site marked with 1 is at    [2][2]
//  |   |   |   |   |
//  v   . - . - . - . -       Row i=3
//      |   |   |   |

void get_bonded_neighbors(int** vbonds, int** hbonds, int M, int N,
	int A1[d], int neighbors[MAXNEI][d], int* pnumnei)
{
	int numnei = 0;
	int A1i    = A1[0];
	int A1j    = A1[1];

	int A1im1 = A1i - 1;
	int A1jm1 = A1j - 1;
	int A1ip1 = A1i + 1;
	int A1jp1 = A1j + 1;

	if (A1im1 <  0) A1im1 += M;
	if (A1jm1 <  0) A1jm1 += N;
	if (A1ip1 >= M) A1ip1 -= M;
	if (A1jp1 >= N) A1jp1 -= N;

	if (vbonds[A1i  ][A1j  ]) { // Down  bond
		neighbors[numnei][0] = A1ip1;
		neighbors[numnei][1] = A1j;
		numnei++;
	}

	if (hbonds[A1i  ][A1j  ]) { // Right bond
		neighbors[numnei][0] = A1i;
		neighbors[numnei][1] = A1jp1;
		numnei++;
	}

	if (vbonds[A1im1][A1j  ]) { // Up    bond
		neighbors[numnei][0] = A1im1;
		neighbors[numnei][1] = A1j;
		numnei++;
	}

	if (hbonds[A1i  ][A1jm1]) { // Left  bond
		neighbors[numnei][0] = A1i;
		neighbors[numnei][1] = A1jm1;
		numnei++;
	}

	*pnumnei = numnei;
}

// ----------------------------------------------------------------
int get_num_neighbors(int** vbonds, int** hbonds, int M, int N, int A1[d])
{
	int neighbors[MAXNEI][d];
	int numnei = -1;
	get_bonded_neighbors(vbonds, hbonds, M, N, A1, neighbors, &numnei);
	return numnei;
}

// ----------------------------------------------------------------
// Algorithm:
//
// mark A1
// get bonded neighbors of A1
// for each bonded neighbor B:
//   if B is not marked:
//     mark_one_cluster(s,v,h,B)
//

void mark_one_cluster(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int mark_value)
{
	fill_matrix(site_marks, M, N, SITECHAR);
	mark_one_cluster_aux(site_marks, vbonds, hbonds, M, N, A1,
		mark_value);
}

void mark_one_cluster_aux(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int mark_value)
{
	int neighbors[MAXNEI][d];
	int numnei;
	int k;

	site_marks[A1[0]][A1[1]] = mark_value;
	get_bonded_neighbors(vbonds, hbonds, M, N, A1, neighbors, &numnei);
	for (k = 0; k < numnei; k++) {
		int Bi = neighbors[k][0];
		int Bj = neighbors[k][1];
		if (site_marks[Bi][Bj] != mark_value)
			mark_one_cluster_aux(site_marks, vbonds, hbonds, M, N,
				neighbors[k], mark_value);
	}
}

// ----------------------------------------------------------------
double get_cluster_size(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d])
{
	int i, j;
	int cluster_size = 0;

	mark_one_cluster(site_marks, vbonds, hbonds, M, N, A1, VISITEDCHAR);
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (site_marks[i][j] == VISITEDCHAR)
				cluster_size++;
	return cluster_size;
}

// ----------------------------------------------------------------
double get_mean_C0_size(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d])
{
	double mean_C0_size = 0.0;
	int C0_size;
	int rep;
	for (rep = 0; rep < reps; rep++) {
		populate_bonds(vbonds, hbonds, M, N, p);
		C0_size = get_cluster_size(site_marks, vbonds, hbonds, M, N, A1);
		mean_C0_size += C0_size;
	}
	mean_C0_size /= reps;
	return mean_C0_size;
}

// ----------------------------------------------------------------
double get_mean_finite_C0_size(
	int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d])
{
	int num_clusters;
	int* cluster_sizes;
	double mean_finite_C0_size = 0.0;
	int A_cluster_size;
	int rep;
	int num_finite = 0;
	int A_clno;
	int largest_clno;

	for (rep = 0; rep < reps; rep++) {
		populate_bonds(vbonds, hbonds, M, N, p);
		mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, &num_clusters);
		cluster_sizes = (int*)malloc_or_die(sizeof(int) * num_clusters);
		get_cluster_sizes(site_marks, M, N, num_clusters, cluster_sizes,
			&largest_clno);
		A_clno = site_marks[A1[0]][A1[1]];
		A_cluster_size = cluster_sizes[A_clno];

#if 0
		if (p < P_C) {
			mean_finite_C0_size += A_cluster_size;
			num_finite++;
		}
		else {
			if (A_clno != largest_clno) {
				mean_finite_C0_size += A_cluster_size;
				num_finite++;
			}
		}
#else
		if (A_clno != largest_clno) {
			mean_finite_C0_size += A_cluster_size;
			num_finite++;
		}
#endif

		free(cluster_sizes);
	}
	if (num_finite == 0)
		mean_finite_C0_size = 0.0;
	else
		mean_finite_C0_size /= num_finite;
	return mean_finite_C0_size;
}

// ----------------------------------------------------------------
// CORRELATION LENGTH
//
// Abstract definition of correlation length xi(p) for bond percolation,
// where C_0 is the cluster containing the origin:
//
//           [  sum_x |x|^2 P(x in C_0)  ]^1/2
//   xi(p) = [ ------------------------- ].
//           [  sum_x       P(x in C_0)  ]
//
// Computational issue:  As is, the sum over lattice sites x is on the outside.
// The probabilities, to be estimated by sample means taken over R
// realizations, are on the inside.  Naively following this recipe, we would
// generate R realizations for each of the MN lattice sites x.  It is far more
// efficient to loop over all x, inside a loop over realizations.  Thus for
// each single realization, we can estimate P(x in C_0) for all x in the
// lattice.
//
// Recall that P(A) = E[1_A].  Then, with E being the expectation over all
// realizations, estimated by sample mean over R realizations, we have
//
//              sum_x |x|^2 P(x in C_0)
//   xi^2(p) = -------------------------
//              sum_x       P(x in C_0)
//
//              sum_x |x|^2 E[ 1_(x in C_0)]
//           = -----------------------------
//              sum_x       E[ 1_(x in C_0)]
//
//              E[ sum_x |x|^2  1_(x in C_0)]
//           = -----------------------------
//              E[ sum_x        1_(x in C_0)]
//
//              1/R sum_{r=1}^R sum_x |x|^2  1_(x in C_0)
//          ~= ------------------------------------------
//              1/R sum_{r=1}^R sum_x        1_(x in C_0)
//
//              sum_{r=1}^R sum_x |x|^2  1_(x in C_0)
//          ~= --------------------------------------
//              sum_{r=1}^R sum_x        1_(x in C_0)
//
//
// This is for p < p_c.  For p > p_c, replace P(x in C_0) with the conditional
// probability P(x in C_0 | #C_0 < infty).

double get_corrlen(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d])
{
	int num_clusters;
	int rep, i, j;
	int A_clno, x_clno, largest_clno;
	int normx2;
	double upper_sum = 0.0;
	double lower_sum = 0.0;
	double upper_term, lower_term;
	int max_num_clusters = M*N; // p=0 case
	int* cluster_sizes = (int*)malloc_or_die(sizeof(int) * max_num_clusters);
	double corrlen;

	for (rep = 0; rep < reps; rep++) {
		populate_bonds(vbonds, hbonds, M, N, p);
		mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, &num_clusters);

		get_cluster_sizes(site_marks, M, N, num_clusters, cluster_sizes,
			&largest_clno);
		A_clno = site_marks[A1[0]][A1[1]];

		upper_term = 0.0;
		lower_term = 0.0;
#if 0
		if ((p < P_C) || (A_clno != largest_clno))
#else
		if (A_clno != largest_clno)
#endif
		{
			for (i = 0; i < M; i++) {
				for (j = 0; j < N; j++) {
					x_clno = site_marks[i][j];

					if (A_clno == x_clno) {
						int di = i - A1[0];
						int dj = j - A1[1];
						normx2 = di*di + dj*dj;
						upper_term += normx2;
						lower_term++;
					}
				}
			}
		}
		upper_sum += upper_term;
		lower_sum += lower_term;
	}

	free(cluster_sizes);

	if (lower_sum == 0.0)
		corrlen = 0.0;
	else
		corrlen = sqrt(upper_sum / lower_sum);
	return corrlen;
}

// ----------------------------------------------------------------
int A1_oo_A2_aux(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d])
{
	int nei[MAXNEI][d];
	int numnei;
	int k;

	if (pteq(A1, A2))
		return 1;
	site_marks[A1[0]][A1[1]] = VISITEDCHAR;
	get_bonded_neighbors(vbonds, hbonds, M, N, A1, nei, &numnei);
	for (k = 0; k < numnei; k++) {
		int Bi = nei[k][0];
		int Bj = nei[k][1];
		if (site_marks[Bi][Bj] != VISITEDCHAR)
			if (A1_oo_A2_aux(site_marks, vbonds, hbonds, M, N, nei[k], A2))
				return 1;
	}
	return 0;
}

int A1_oo_A2(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int A1[d], int A2[d])
{
	fill_matrix(site_marks, M, N, SITECHAR);
	return A1_oo_A2_aux(site_marks, vbonds, hbonds, M, N, A1, A2);
}

// ----------------------------------------------------------------
double P_A1_oo_A2(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, double p, int reps, int A1[d], int A2[d])
{
	double nctd = 0.0;
	int rep;
	for (rep = 0; rep < reps; rep++) {
		populate_bonds(vbonds, hbonds, M, N, p);
		if (A1_oo_A2(site_marks, vbonds, hbonds, M, N, A1, A2))
			nctd += 1.0;
	}
	return nctd / reps;
}

// ----------------------------------------------------------------
void mark_cluster_numbers(int** site_marks, int** vbonds, int** hbonds,
	int M, int N, int* pnum_clusters)
{
	int neighbors[MAXNEI][d];
	int numnei;
	int i, j, m;
	int A[d];
	int cluster_number = 0;

	fill_matrix(site_marks, M, N, -1);

	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			if (site_marks[i][j] >= 0) // Already marked
				continue;

			A[0] = i;
			A[1] = j;
			get_bonded_neighbors(vbonds, hbonds, M, N, A, neighbors, &numnei);

			site_marks[i][j] = cluster_number;
			for (m = 0; m < numnei; m++) {
				int Bi = neighbors[m][0];
				int Bj = neighbors[m][1];
				if (site_marks[Bi][Bj] == -1) {
					mark_one_cluster_aux(site_marks, vbonds, hbonds, M, N,
						neighbors[m], cluster_number);
				}
			}
			cluster_number++;
		}
	}
	if (pnum_clusters)
		*pnum_clusters = cluster_number;
}

// ----------------------------------------------------------------
void sanity_check_cluster_numbers(int** site_marks, int** vbonds, int** hbonds,
	int M, int N)
{
	int i, j, k;
	int A[d];
	int nei[MAXNEI][d];
	int numnei;
	int clunoA, clunoB;

	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			A[0] = i;
			A[1] = j;
			get_bonded_neighbors(vbonds, hbonds, M, N, A, nei, &numnei);
			clunoA = site_marks[i][j];
			for (k = 0; k < numnei; k++) {
				clunoB = site_marks[nei[k][0]][nei[k][1]];
				if (clunoA != clunoB) {
					printf("Cluster mismatch!!\n");
					exit(1);
				}
			}
		}
	}
}

// ----------------------------------------------------------------
void get_cluster_sizes(int** site_marks, int M, int N, int num_clusters,
	int* cluster_sizes, int* pC_clno)
{
	int i, j, k, clno;
	int largest = 0;

	for (k = 0; k < num_clusters; k++)
		cluster_sizes[k] = 0;
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			clno = site_marks[i][j];
			cluster_sizes[clno]++;
		}
	}

	largest = 0;
	for (k = 0; k < num_clusters; k++) {
		if (cluster_sizes[k] > cluster_sizes[largest])
			largest = k;
	}
	*pC_clno = largest;
}

// ----------------------------------------------------------------
int A_in_C(int** site_marks, int** vbonds, int** hbonds, int M, int N,
	double p, int A[d], int* cluster_sizes)
{
	int num_clusters;
	int C_clno; // Number of largest cluster
	int A_clno; // Number of cluster containing site A

	mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, &num_clusters);
	get_cluster_sizes(site_marks, M, N, num_clusters, cluster_sizes, &C_clno);
	A_clno = site_marks[A[0]][A[1]];
	if (A_clno == C_clno)
		return 1;
	else
		return 0;
}

// ----------------------------------------------------------------
double P_A_in_C(int** site_marks, int** vbonds, int** hbonds, int M, int N,
	double p, int reps, int A[d])
{
	int k;
	int num_A_in_C = 0;
	int* cluster_sizes = (int*)malloc_or_die(sizeof(int) *M*N);

	for (k = 0; k < reps; k++) {
		populate_bonds(vbonds, hbonds, M, N, p);
		num_A_in_C += A_in_C(site_marks, vbonds, hbonds, M, N, p, A,
			cluster_sizes);
	}

	free(cluster_sizes);
	return (double)num_A_in_C/(double)reps;
}

// ----------------------------------------------------------------
int A1_or_A2_in_C(int** site_marks, int** vbonds, int** hbonds, int M, int N,
	double p, int A1[d], int A2[d], int* cluster_sizes)
{
	int num_clusters;
	int C_clno; // Number of largest cluster
	int A1_clno, A2_clno;

	mark_cluster_numbers(site_marks, vbonds, hbonds, M, N, &num_clusters);
	get_cluster_sizes(site_marks, M, N, num_clusters, cluster_sizes,
		&C_clno);
	A1_clno = site_marks[A1[0]][A1[1]];
	A2_clno = site_marks[A2[0]][A2[1]];
	if (A1_clno == C_clno)
		return 1;
	if (A2_clno == C_clno)
		return 1;
	return 0;
}

// ----------------------------------------------------------------
double P_A1_or_A2_in_C(int** site_marks,
	int** vbonds, int** hbonds, int M, int N,
	double p, int reps, int A1[d], int A2[d])
{
	int k;
	int num_A1_or_A2_in_C = 0;
	int* cluster_sizes = (int*)malloc_or_die(sizeof(int) *M*N);

	for (k = 0; k < reps; k++) {
		populate_bonds(vbonds, hbonds, M, N, p);
		num_A1_or_A2_in_C += A1_or_A2_in_C(site_marks, vbonds, hbonds,
			M, N, p, A1, A2, cluster_sizes);
	}

	free(cluster_sizes);
	return (double)num_A1_or_A2_in_C/(double)reps;
}
