// ================================================================
// URANDOM.C
// Please see the comments in urandom.h for information.
// ================================================================

// ================================================================
// John Kerl
// kerl.john.r@gmail.com
// 2008-04-29
//
// These are wrapper routines for obtaining pseudorandom integers from the
// Linux /dev/urandom stream.
// ================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "urandom.h"

#define BUFSZ 2048

// ----------------------------------------------------------------
int get_urandom(void)
{
	static int fd = -1;
	static int buf[BUFSZ];
	static int bufpos = 0;
	static char * dev_name = "/dev/urandom";
	int bytes_wanted;
	int bytes_read;
	int rv;
	if (fd == -1) {
		fd = open(dev_name, O_RDONLY);
		if (fd < 0) {
			perror("open");
			fprintf(stderr, "Couldn't open %s.\n", dev_name);
			exit(1);
		}
	}

	if (bufpos == 0) {
		bytes_wanted = BUFSZ * sizeof(int);
		bytes_read   = read(fd, buf, BUFSZ * sizeof(int));
		if (bytes_read != bytes_wanted) {
			perror("read");
			fprintf(stderr, "Couldn't read %s.\n", dev_name);
			fprintf(stderr, "Expected %d byte(s); got %d.\n",
				bytes_wanted, bytes_read);
			exit(1);
		}
	}
	rv = buf[bufpos];
	bufpos++;
	if (bufpos >= BUFSZ)
		bufpos = 0;
	return rv;
}

// ----------------------------------------------------------------
double get_urandomu(void)
{
	unsigned u = get_urandom();
	double numer = (double)u;
	double denom = 4294967296.0;
	return (double)(numer/denom);
}
