#!/bin/bash

for FLE in `ls src`
do
  echo $FLE
  ./opt_run.sh $FLE
done