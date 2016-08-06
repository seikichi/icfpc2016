#!/bin/bash

set -ux

./tools/download_problems.py

(cd specview; make)

for f in problems/*.in; do
    id=$(echo $f | sed -E 's@problems/([[:digit:]]+).in@\1@')
    ./specview/specview < problems/$id.in > problems/$id.svg
done

git add problems/*.in problems/*.svg

if git diff --cached --name-only | grep -q problems ; then
    git commit -m 'Update problems (by Jenkins)'
fi
