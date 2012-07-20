// ================================================================
// PSDES.H
//
// The following is a pseudorandom sequence generator, loosely inspired by DES,
// adapted from _Numerical Recipes in C_.
//
// John Kerl
// kerl.john.r@gmail.com
// 2004-08-14
//
// ================================================================
// The key function is a 64-bit-to-64-bit hash.  The former 64 bits are the
// generator state; the latter are the generator output:
//
// * Seeding the generator means setting the 64-bit state.
//
// * Stepping the generator runs the hash on the 64-bit state, then
//   incrementing the state, then returning the hash output.
//
// * The generator thus has a period of 2^64.
//
// Example:  Suppose the state is set to 0x00000002fffffffc.  The generator
// is run 6 times, with the following output:
//
//   State               Output
//   ------------------  ------------------
//   0x00000002fffffffc  0x57733ef0998adaea
//   0x00000002fffffffd  0xe486359a3e59f158
//   0x00000002fffffffe  0x624cb3680dd3090c
//   0x00000002ffffffff  0x5e920cb5bfc315ce
//   0x0000000300000000  0xadcd49eef6f27d08
//   0x0000000300000001  0x786f6a22d7df7670
//
// The state will now be 0x0000000300000002.
//
// ================================================================
// Details:
//
// * 64-bit integers are represented as a 32-bit pair.
//
// * Several functions are supplied, some of which only return the bottom
//   32 bits of the generator output.
//
// * Non-reentrant and reentrant versions are supplied.  The former are
//   simpler to call; the generator state is remembered for you.  For the
//   latter, you keep the state yourself.  (However, the generator will
//   still increment your state each time it is called.)  Reentrant versions
//   end in "_r".
//
// * For the reentrant versions, you must seed the generator (i.e. assign
//   values to the two 32-bit state values) yourself.  For convenience,
//   a function (sran32_tod_r) is supplied, which you can use to obtain a
//   seed value which will probably be different on each call.
//
// * For the non-reentrant version, you may seed the generator if you wish,
//   but you do not need to.  The generator remembers if its state has been
//   seeded -- if not, it sets a time-of-day seed the first time it is called.
//
// ================================================================
// Sample usage:

// * Non-reentrant version with default seeds:
//
//   unsigned rand;
//   rand = iran32();
//   rand = iran32();
//   rand = iran32();
//
//   double rand;
//   rand = fran32();
//   rand = fran32();
//   rand = fran32();

// * Non-reentrant version with specified seeds:
//
//   unsigned rand;
//   sran32(1);
//   // Or, sran32b(0, 1);
//   rand = iran32();
//   rand = iran32();
//   rand = iran32();
//
//   double rand;
//   sran32(1);
//   // Or, sran32b(0, 1);
//   rand = fran32();
//   rand = fran32();
//   rand = fran32();

// * Reentrant version with time-of-day seeds:
//
//   unsigned state0, state1, rand;
//   sran32_tod_r(1, &state0, &state1);
//   rand = iran32(&state0, &state1);
//   rand = iran32(&state0, &state1);
//   rand = iran32(&state0, &state1);
//
//   unsigned state0, state1;
//   double rand;
//   sran32(1);
//   rand = fran32(&state0, &state1);
//   rand = fran32(&state0, &state1);
//   rand = fran32(&state0, &state1);

// * Reentrant version with specified seeds:
//
//   unsigned state0, state1, rand;
//   state0 = 7;
//   state1 = 8;
//   rand = iran32(&state0, &state1);
//   rand = iran32(&state0, &state1);
//   rand = iran32(&state0, &state1);
//
//   unsigned state0, state1;
//   double rand;
//   state0 = 7;
//   state1 = 8;
//   rand = fran32(&state0, &state1);
//   rand = fran32(&state0, &state1);
//   rand = fran32(&state0, &state1);
// ================================================================

#ifndef PSDES_H
#define PSDES_H

// ----------------------------------------------------------------
// These versions are non-reentrant.
// Usage:  Nominally, just call iran32() or fran32().  They remember whether
// or not a seed has been supplied, and call sran32_tod() if not.  Use sran32()
// only if you want to force the same generator output each time.

// Uniformly distributed pseudorandom 32-bit integer.
unsigned iran32(void);

// Uniformly distributed pseudorandom 64-bit integer.
void     iran64(unsigned * pout0, unsigned * pout1);

// Uniformly distributed single-precision double between 0.0 and 1.0.
double    fran32(void);

// Sets lower 32 bits of generator state to the specified value, and sets
// the upper 32 bits of generator state to 0.
void     sran32(unsigned s);

// Sets all 64 bits of generator state to the specified values.
void     sran32b(unsigned s0, unsigned s1);

// Sets all 64 bits of generator state to the values dependent on the
// Unix PID, time of day in seconds, and time of day in microseconds.
void     sran32_tod(void);

// ----------------------------------------------------------------
// These versions are reentrant.

// Uniformly distributed pseudorandom 32-bit integer.
unsigned iran32_r(unsigned * pstate0, unsigned * pstate1);

// Uniformly distributed pseudorandom 64-bit integer.
void     iran64_r(unsigned * pout0, unsigned * pout1,
	unsigned * pstate0, unsigned * pstate1);

// Uniformly distributed single-precision double between 0.0 and 1.0.
double    fran32_r(unsigned * pstate0, unsigned * pstate1);

// There is no sran32_r() function.  You own the state variables and may
// assign to them whatever values you wish.

// This puts time-of-day information into your state variables.
void     sran32_tod_r(unsigned * pstate0, unsigned * pstate1void);

// ----------------------------------------------------------------
// This is the 64-bit pseudo-DES in-place hash.
void psdes_hash_64(
	unsigned * pword0,
	unsigned * pword1);

#endif // PSDES_H
