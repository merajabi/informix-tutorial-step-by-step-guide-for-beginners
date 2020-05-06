#!/bin/bash

FILENAME=$1
LINES=$2

export INFORMIXSERVER=informix
export INFORMIXDIR=/opt/ibm/informix
export LD_LIBRARY_PATH="$INFORMIXDIR/lib:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib/c++:$INFORMIXDIR/lib/dmi:$INFORMIXDIR/lib/cli"

IFX_DIFINED="-DLINUX -DIT_HAS_DISTINCT_LONG_DOUBLE -DIT_COMPILER_HAS_LONG_LONG -DIT_DLLIB -DMITRACE_OFF"
IFX_INCPATH="-I$INFORMIXDIR/incl -I$INFORMIXDIR/incl/c++ -I$INFORMIXDIR/incl/dmi -I$INFORMIXDIR/incl/esql"
IFX_LIBPATH="-L$INFORMIXDIR/lib -L$INFORMIXDIR/lib/c++ -L$INFORMIXDIR/lib/dmi -L$INFORMIXDIR/lib/esql"
IFX_LIBS="-lifc++  -lifdmi   -lifsql -lifasf -lifgen -lifos -lifgls -lifglx $INFORMIXDIR/lib/esql/checkapi.o"
LIBS="-lcrypt -lnsl -lm -ldl"

rm -f *.out
(/usr/bin/g++ -g -std=c++11  $IFX_DIFINED -fPIC -fsigned-char $IFX_INCPATH -c $FILENAME.cpp) || exit 1;
(/usr/bin/g++ -o $FILENAME.out $FILENAME.o $IFX_LIBPATH $IFX_LIBS $LIBS -Wl,-rpath,$LD_LIBRARY_PATH) || exit 1;

rm -f *.o

if [ "X$LINES" = "X" ];then
	SHOW="tail -n 18"
elif [ "X$LINES" = "X0" ];then
	SHOW="cat"
else
	SHOW="tail -n $LINES"
fi

echo;
echo "***	valgrind Output Summary:";
echo "---------------------------------------------------";
valgrind -v --leak-check=full ./$FILENAME.out 2>&1 | $SHOW

echo;
echo "***	Program Output:";
echo "---------------------------------------------------";
./$FILENAME.out

exit 0;
