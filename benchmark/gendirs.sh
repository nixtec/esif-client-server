#!/bin/sh -x

i=1025

while [ $i -lt 3000 ] ; do
  i=$((i+1))
  cp -r INSTITUTE_1025 INSTITUTE_$i
done
