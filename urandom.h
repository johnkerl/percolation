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
//
// This code is released under the terms of the GNU GPL.  Please see
// COPYING.txt in the same directory as this file.
// ================================================================

#ifndef URANDOM_H
#define URANDOM_H

int    get_urandom (void);
double get_urandomu(void);

#endif // URANDOM_H
