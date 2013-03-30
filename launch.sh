#!/bin/bash

nbteams=$1
nbplayers=$2

if [ $nbteams -lt 0 ] || [ $nbplayers -lt 0 ]
then
  exit 0
fi


for j in `seq 1 $nbplayers`
do
  for i in `seq 1 $nbteams`
  do
    ./lemipc $i &
  done
done
