#!/bin/bash

function usage() {
	echo "Usage: ./opt_run.sh main.c"
}

if test $# -ne 1
then
	usage
	exit 1
fi

rm -rf {bc,tmps}/*

cd bc
for FLE in `find ../src |grep $1`
do
	echo $FLE
	clang -emit-llvm -c $FLE
	FLX=`echo $FLE |sed 's/\.c/\.bc/' | sed 's/src/bc/'`
	cd ../tmps
	echo $FLX
	../../bin/opt -load ../../lib/LLVMUfmg.so -ufmg -enable-new-pm=0 < $FLX > /dev/null
	cd ../bc
done
cd ..

cd pdfs
rm -rf $1
mkdir $1
cd $1
i=0
for FLE in `ls ../../tmps/*.udot`
do
	pwd
	echo "dot -Tpdf $FLE -o $1.$i.pdf"
	dot -Tpdf $FLE -o $1.$i.pdf
	i=$((i + 1))
done
cd ..
cd ..

tree
cd pdfs/$1
for FLE in `ls`
do
	echo "xpdf $FLE"
	xpdf $FLE
done
cd ../..
