================================================================
John Kerl
Jan. 20, 2010

================================================================
PURPOSE / RESEARCH QUESTIONS

This code serves two purposes.

(1) In a spring 2008 independent study under Jan Wehr, I examined 2D
    and 3D percolation on integer lattices.  The larger context involved
    quantum entanglement swapping on lattices; however, the computational
    questions reduced to the following:

    * Given a lattice site A, what is the probability that the site is in the
      (unique) infinite cluster on the infinite lattice?  For a computational
      approximation with an MxN lattice, we estimate the probablity that A is
      in the largest cluster.  This is an old question; see Grimmett's
      _Percolation_ for more information.  I chose to examine this numerically,
      even though the question is not new, to sanity-check my code against
      known results.

    * Following on the article "Entanglement Distribution in Pure-State
      Quantum Networks", Perseguers, Cirac, Acın, Lewenstein, and Wehr,
      arXiv:0708.1025v2, given two lattice sites at opposite corners of a unit
      square, estimate the probablity that one of the two sites is in the
      largest cluster.  This was treated by John LaPeyre in the Perseguers et
      al. paper; again, I wanted to reproduce known results as a sanity check.

    * Extending the Perseguers et al. paper, address a similar question for
      3D lattices.  For more information about the research., please also see
      frgslides.pdf in the same directory as this file.

(2) This project satisfies the computational requirement for my PhD at the
    University of Arizona, examined by Kevin Lin.  My dissertation work is
    largely computational; however, I chose to use this side project for my
    computing exam since it involves fewer lines of code.  Here, for brevity, I
    am only presenting the 2D code.

================================================================
PROBABILITIES OF INTEREST FOR THE RESEARCH PROJECT

Let A be the origin; let A1 be the point (1,0) and A2 be (0,1).  On the
infinite lattice (all of Z^2), there is a unique infinite cluster C with
probability 1 whenever p > p_c = 0.5.  On the finite MxN lattice, there is a
largest cluster C.  (As long as M and N are larger than the p-dependent
correlation length, that largest cluster probably is contained in the infinite
cluster.)

* theta = P(A in C)

* sigma = P(A1 in C or A2 in C)

* tau   = P(A1 o--o A2), where A1 o--o A2 means there is some path
  from A1 to A2.

================================================================
HOW TO COMPILE

You should be able to simply type "make" on any reasonably configured Linux
system.

* For debugging with gdb, please type "make clean debug".

* For profiling with gprof, please type "make clean profile".  Then, execute
  the perco2 program as described in the "HOW TO EXECUTE" section below.  Then,
  type "gprof ./perco2 gmon.out > profile.txt" and view the file profile.txt to
  see where the program is spending its time.

* For efficient execution without support for gdb or gprof, please type "make
  clean opt" or "make clean; make".

================================================================
HOW TO EXECUTE

After compiling with "make", type "./perco2" with no arguments to get a usage
reminder.

Examples of invoking perco2 a single time, interactively:

* ./perco2 print        p=0.6 MN=20
  Plots a lattice to the screen using ASCII art.

* ./perco2 plot         p=0.6 MN=200
  Creates "p2.ppm", which is an image file.

* ./perco2 nei          p=0.6 MN=20
  Tests out the number-of-neighbors computation, using visual inspection.

* ./perco2 cluster      p=0.6 MN=20
  Tests out the single-cluster-marking computation, using visual inspection.

* ./perco2 plotcluster  p=0.6 MN=200
  Creates "p2.ppm", which is an image file.

* ./perco2 meanC0size   p=0.6 MN=20 reps=10000

* ./perco2 meanfC0size  p=0.6 MN=20 reps=10000

* ./perco2 corrlen      p=0.6 MN=20

* ./perco2 1o2          p=0.6 MN=20
  Tests out the A1 o--o A2 computation, using visual inspection.

* ./perco2 P1o2         p=0.6 MN=20 reps=10000
  Estimates tau(p), i.e. P(A1 o--o A2).

* ./perco2 clnos        p=0.6 MN=20
  Tests out the all-cluster-marking computation, using visual inspection.

* ./perco2 plotclusters p=0.6 MN=200
  Creates "p2.ppm", which is an image file.

* ./perco2 clszs        p=0.6 MN=20
  Tests out the cluster-sizes computation, using visual inspection.

* ./perco2 AinC         p=0.6 MN=20
  Tests out the largest-cluster-membership computation, using visual
  inspection.

* ./perco2 PAinC        p=0.6 MN=20 reps=10000
  Estimates theta(p), i.e. P(A in C).

* ./perco2 U2inC        p=0.6 MN=20

* ./perco2 PU2inC       p=0.6 MN=20 reps=10000

Example of invoking perco2 in a shell script:  please see greeks.sh.

================================================================
ABOUT THE CODE

Please see the comments in greeks.sh, which invokes the C program perco2
repeatedly, sweeping over the parameter space (M, N, p).

Top-level comments about the C code may be found in perco2lib.h.
