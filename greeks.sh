#!/bin/sh

# ================================================================
# This script repeatedly invokes the perco2 executable for each combination of
# parameters (M, N, p).  This script will print the estimator of the selected
# random variable to the screen.  That output may be directed to a file for
# further analysis.
#
# Example output of this script:
#
#   kerl@sprax% greeks.sh tau
#   M=20 N=20 p=0.4500 reps=10000 PA1ooA2=  0.5214000
#   M=20 N=20 p=0.4500 reps=10000 PA1ooA2=  0.5194000
#   M=20 N=20 p=0.4500 reps=10000 PA1ooA2=  0.5278000
#   M=20 N=20 p=0.4520 reps=10000 PA1ooA2=  0.5218000
#   M=20 N=20 p=0.4520 reps=10000 PA1ooA2=  0.5218000
#   :
#   :
#
# Events considered (see README.txt):
# * theta = P(A in C)
# * sigma = P(A1 in C or A2 in C)
# * tau   = P(A1 o--o A2)
# ================================================================
# John Kerl
# kerl.john.r@gmail.com
# 2010-01-22
# ================================================================

ps=" \
0.450 0.452 0.454 0.456 0.458 0.460 0.462 0.464 0.466 0.468 \
0.470 0.472 0.474 0.476 0.478 0.480 0.482 0.484 0.486 0.488 \
0.490 0.492 0.494 0.496 0.498 0.500 0.502 0.504 0.506 0.508 \
0.510 0.512 0.514 0.516 0.518 0.520 0.522 0.524 0.526 0.528 \
0.530 0.532 0.533 0.535 0.537 0.540 0.542 0.544 0.546 0.548"

MNs="20 30 40 50 60 70 80 90 100"
reps=10000
tries="1 2 3"

# E.g. one may type "greeks.sh theta", "greeks.sh sigma", "greeks.sh tau".
if [ $# -ne 1 ]; then
	echo "Usage: $0 {theta|sigma|tau}" 1>&2
	exit 1
fi
greek=$1

if   [ $greek = theta ]; then
	cmd=PAinC
elif [ $greek = sigma ]; then
	cmd=PU2inC
elif [ $greek = tau ]; then
	cmd=P1o2
else
	echo "Unrecognized command \"$cmd\"." 1>&2
	exit 1
fi

for MN in $MNs; do
	for p in $ps; do
		for try in $tries; do
			./perco2 $cmd reps=$reps MN=$MN p=$p
		done
	done
done
