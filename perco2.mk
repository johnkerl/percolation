# ================================================================
# Makefile for project perco2
# Automatically generated from "perco2.mki" at Fri Jun 12 16:23:39 2009

# yamm v1.0
# John Kerl
# 2002/05/04
# ================================================================


INCLUDE_DIRS =
LIB_DIRS =
DEFINES =
MISC_CFLAGS =
MISC_LFLAGS =
EXTRA_DEPS =
COMPILE_FLAGS = -c $(INCLUDE_DIRS) $(DEFINES) $(MISC_CFLAGS)
LINK_FLAGS =  $(LIB_DIRS) $(MISC_LFLAGS)

build: mk_obj_dir ./perco2

mk_obj_dir:
	mkdir -p ./perco_objs

./perco_objs/perco2.o:  perco2.c perco2lib.h perco2plot.h perco2print.h psdes.h putil.h rcmrand.h urandom.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  perco2.c -o ./perco_objs/perco2.o

./perco_objs/perco2lib.o:  perco2lib.c perco2lib.h perco2print.h psdes.h putil.h rcmrand.h urandom.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  perco2lib.c -o ./perco_objs/perco2lib.o

./perco_objs/perco2print.o:  perco2lib.h perco2print.c perco2print.h psdes.h putil.h rcmrand.h urandom.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  perco2print.c -o ./perco_objs/perco2print.o

./perco_objs/perco2plot.o:  perco2lib.h perco2plot.c psdes.h putil.h rcmrand.h rgb_matrix.h urandom.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  perco2plot.c -o ./perco_objs/perco2plot.o

./perco_objs/rgb_matrix.o:  putil.h rgb_matrix.c rgb_matrix.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  rgb_matrix.c -o ./perco_objs/rgb_matrix.o

./perco_objs/psdes.o:  psdes.c psdes.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  psdes.c -o ./perco_objs/psdes.o

./perco_objs/urandom.o:  urandom.c urandom.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  urandom.c -o ./perco_objs/urandom.o

./perco_objs/putil.o:  putil.c putil.h
	gcc $(OPTCFLAGS) -Wall -Werror $(COMPILE_FLAGS)  putil.c -o ./perco_objs/putil.o

OBJS = \
	./perco_objs/perco2.o \
	./perco_objs/perco2lib.o \
	./perco_objs/perco2print.o \
	./perco_objs/perco2plot.o \
	./perco_objs/rgb_matrix.o \
	./perco_objs/psdes.o \
	./perco_objs/urandom.o \
	./perco_objs/putil.o

./perco2: $(OBJS) $(EXTRA_DEPS)
	gcc $(OPTLFLAGS) -lm $(OBJS) -o ./perco2 $(LINK_FLAGS)

clean:
	-@rm -f $(OBJS)
	-@rm -f ./perco2
