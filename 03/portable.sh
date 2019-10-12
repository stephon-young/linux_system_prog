#!/bin/bash

# for compile by -D option.

out=a.out

if [ -z "$1" ]; then
  gcc -o $out portable.c
elif [ "$1" == "-x" ]; then
  xopen=100
  if [ -n "$2" ]; then
    xopen=$2
  fi
  if [[ "$xopen" != "500" && "$xopen" != "600" && "$xopen" != "700" ]]; then
    echo "invalid -x option, you must be in <500,600,700>"
    exit 1
  fi
  gcc -D_XOPEN_SOURCE=${xopen} -o $out portable.c 
else
  echo "unknown option."
  exit 2
fi

echo "compile output: $out successful, execute ./${out} for test".
exit 0