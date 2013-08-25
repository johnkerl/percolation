// ================================================================
// URANDOM.H
//
// These are nothing more than wrapper routines for obtaining pseudorandom
// integers from the Linux /dev/urandom stream.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
// ================================================================

#ifndef URANDOM_H
#define URANDOM_H

int    get_urandom (void);
double get_urandomu(void);

#endif // URANDOM_H
