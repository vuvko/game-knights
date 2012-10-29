#!/bin/bash

players=( ./game-knights "wine gamer1v2.exe" )
i=0

cat matrix.in > matrix.txt
echo "start" > matrix.log

while [[ -n "cat matrix.txt | head -1 | egrep '.* (A|B|D)'" ]]; do
  cat matrix.txt >> matrix.log
  ./game-knights
  #${players[$i]}
  #((i^=1))
  echo "********* ${i} ***********" >> matrix.log
  echo "----"
  cat matrix.txt
  cat matrix.txt | head -1 | egrep '.* (A|B|D)'
done
