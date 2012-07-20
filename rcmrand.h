// ================================================================
// RCMRAND.H
//
// This is a lightweight abstraction layer which allows me to experiment with
// various pseudorandom-number generators, while making no code changes other
// than in this file each time I want to switch generators.
//
// The generators currently being abstracted here are:
// * pseudo-DES from Numerical Recipes, 2nd ed.
// * rand48() from the standard C library.
// * The Linux /dev/urandom stream.
//
// The uses to which the generators are put in this software
// application are:
// * SRANDOM():      seed from 32-bit value.
// * URANDOM():      uniform double on [0.0, 1.0).
// * IMODRANDOM(m):  uniform int on {0, 1, 2, ..., m-1}.
// * RANDRANGE(lo,hi): uniform double on [lo, hi).
// * RANDPM():       uniform on {+1, -1}.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-02-02
//
// This code is released under the terms of the GNU GPL.  Please see
// COPYING.txt in the same directory as this file.
// ================================================================

#ifndef RCMRAND_H
#define RCMRAND_H

// The pseudorandom number generator(s) being abstracted:
#include <stdlib.h> // rand48

#include <sys/types.h> // For time(0) and getpid()
#include <sys/unistd.h>
#include <time.h>

#include "psdes.h"  // pseudo-DES

#include "urandom.h" // Linux /dev/urandom with caching.

#define RCM_PSDES   1
#define RCM_URANDOM 2
#define RCM_RAND48  3

// ================================================================
// Here is where one specifies the RNG.  All but one of the following
// should be commented out.  The rest of this file should not need changing.

//#define RCM_WHICH RCM_PSDES
//#define RCM_WHICH RCM_URANDOM
#define RCM_WHICH RCM_RAND48

// ================================================================
// pseudo-DES.
// It has 64-bit state but accepts a 32-bit seed.
#if RCM_WHICH == RCM_PSDES
#define RCM_RAND_DESC "psdes"
#define STRANDOM(s)
#define SRANDOM(s)    sran32((unsigned)(s))
#define URANDOM()     fran32()
#define IMODRANDOM(m) (iran32() % (m))
#endif

// ----------------------------------------------------------------
// /dev/urandom
#if RCM_WHICH == RCM_URANDOM
#define RCM_RAND_DESC "/dev/urandom"
#define STRANDOM(s)   // No such needed.
#define SRANDOM(s)    // No such needed.
#define URANDOM()     get_urandomu()
#define IMODRANDOM(m) ((int)((unsigned)get_urandom() % (m)))
#endif

// ----------------------------------------------------------------
// rand48.
// It has 48-bit state but accepts a 32-bit seed.
#if RCM_WHICH == RCM_RAND48
#define RCM_RAND_DESC "rand48"
#define STRANDOM(s)   srand48((long)(time(0) ^ getpid()))
#define SRANDOM(s)    srand48((long)(s))
#define URANDOM()     drand48()
#define IMODRANDOM(m) ((int)(lrand48() % (m)))
#endif

// ----------------------------------------------------------------
#define RANDRANGE(lo,hi) (lo+(hi-lo)*URANDOM())
#define RANDPM()         (URANDOM() < 0.5 ? 1 : -1)

// ----------------------------------------------------------------
#endif // RCMRAND_H
