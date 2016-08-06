#!/bin/bash

./tools/download_problems.py

(cd specview; make)

for f in problems/*.in; do
    id=$(echo $f | sed -E 's@problems/([[:digit:]]+).in@\1@')
    ./specview/specview < problems/$id.in > problems/$id.svg
done

if git diff --name-only | grep -q problems ; then
    git add problems/*.in problems/*.svg
    git commit -m 'Update problems (by Jenkins)'
fi
