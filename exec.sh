#!/bin/sh
make
FILES="testes/*.txt"
if [ $? -eq 0 ] ; then
  for f in $FILES
  do
    echo "\n"
    echo "Lendo arquivo: $f"
    ./teste $f
    echo "\n"
  done
else 
  echo "failed to compile"
fi
make clean