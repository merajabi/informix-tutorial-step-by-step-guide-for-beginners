#!/bin/bash

FILENAME=$1
TEST=$2

export INFORMIXSERVER=informix
export INFORMIXDIR=/opt/IBM/Informix_Client-SDK
export LD_LIBRARY_PATH="$INFORMIXDIR/lib:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib/c++:$INFORMIXDIR/lib/dmi:$INFORMIXDIR/lib/cli"

rm *.out
#make queryex

/usr/bin/g++ -g -std=c++11 -DLINUX -DIT_HAS_DISTINCT_LONG_DOUBLE -DIT_COMPILER_HAS_LONG_LONG -DIT_DLLIB -DMITRACE_OFF -fPIC -fsigned-char -I$INFORMIXDIR/incl/c++ -I$INFORMIXDIR/incl/dmi -I$INFORMIXDIR/incl -I$INFORMIXDIR/incl/esql -c $FILENAME.cpp

/usr/bin/g++ -g -std=c++11 -DLINUX -DIT_HAS_DISTINCT_LONG_DOUBLE -DIT_COMPILER_HAS_LONG_LONG -DIT_DLLIB -DMITRACE_OFF -fPIC -fsigned-char -I$INFORMIXDIR/incl/dmi -I$INFORMIXDIR/incl -I$INFORMIXDIR/incl/esql -o $FILENAME.out $FILENAME.o  -L$INFORMIXDIR/lib/c++ -lifc++ -L$INFORMIXDIR/lib/dmi -lifdmi  -L$INFORMIXDIR/lib/esql -L$INFORMIXDIR/lib -lifsql -lifasf -lifgen -lifos -lifgls -lifglx $INFORMIXDIR/lib/esql/checkapi.o -lm -ldl -lcrypt -lnsl -Wl,-rpath,$LD_LIBRARY_PATH

if [ "X$TEST" != "X0" ];then
	valgrind -v --leak-check=full ./$FILENAME.out
fi
rm *.o

