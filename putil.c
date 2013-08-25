// ================================================================
// PUTIL.C
// Please see the comments in putil.h for information.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
// ================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "putil.h"

// ----------------------------------------------------------------
void* malloc_or_die(int num_bytes)
{
	void* rv = malloc(num_bytes);
	if (rv == 0) {
		fprintf(stderr, "malloc(%d) failed.\n", num_bytes);
		exit(1);
	}
	return rv;
}

// ----------------------------------------------------------------
char* get_sys_time_string(void)
{
	struct timeval now;
	if (gettimeofday(&now, 0) < 0) {
		perror("gettimeofday");
		exit(1);
	}
	return ctime(&now.tv_sec);
}

// ----------------------------------------------------------------
// If there is a file in the current directory called "__stop__", then quit.
void check_stop(void)
{
	struct stat statbuf;
	if (stat("./__stop__", &statbuf) == 0) {
		printf("Exiting due to __stop__ file.  Time:  %s",
			get_sys_time_string());
		exit(1);
	}
}
