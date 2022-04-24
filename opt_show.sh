#!/bin/bash

function usage() {
	echo "Usage: ./opt_run.sh main.c"
}

if test $# -ne 1
then
	usage
	exit 1
fi

cd pdfs/$1
for FLE in `ls`
do
	echo "xpdf $FLE"
	xpdf $FLE
done
cd ../..