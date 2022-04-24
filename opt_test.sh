#!/bin/bash

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