#!/bin/bash

if test $# -ne 0
then
  if test $# -ne 1
  then
    echo "./opt_test <s>"
    exit
  fi
  if test $1 != "s"
  then
    echo "./opt_test <s>"
    exit
  fi
fi

for FLE in `ls src`
do
  echo $FLE
  if test $1 = "s"
  then
   ./opt_show.sh $FLE 
  else
    ./opt_run.sh $FLE
  fi
done