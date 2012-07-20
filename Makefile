# ================================================================
# John Kerl
# kerl.john.r@gmail.com
# 2009-06-12
# ================================================================

# Compilation optimized for speed.
opt:
	export OPTCFLAGS="-O3"        OPTLFLAGS="";       make -ef perco2.mk

# Compilation with debug symbols.
debug:
	export OPTCFLAGS="-g"         OPTLFLAGS="-g";     make -ef perco2.mk

# Compilation with profiling code.
# After doing "make clean profile", run perco2 with the desired arguments.
# Then, "gprof perco2 gmon.out > profile.txt" and edit profile.txt to see where
# the program is spending its time.
profile:
	export OPTCFLAGS="-g -O3 -pg" OPTLFLAGS="-g -pg"; make -ef perco2.mk

clean:
	make -f perco2.mk clean

tags: .PHONY
	ctags *.[ch]

.PHONY:

over: clean opt
