// ================================================================
// PUTIL.H
//
// These are miscellaneous utility routines.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
//
// This code is released under the terms of the GNU GPL.  Please see
// COPYING.txt in the same directory as this file.

#ifndef PUTIL_H
#define PUTIL_H

// Allocates a specified number of bytes, printing a message to stderr
// and aborting the process if the request fails.
void* malloc_or_die(int num_bytes);

// A keystroke-saving wrapper around gettimeofday() and ctime().
char* get_sys_time_string(void);

// Aborts the process if a file called "__stop__" exists in the current
// directory.  This is a useful way to kill multi-processor jobs.
void check_stop(void);

#endif // PUTIL_H
