#!/bin/bash

./tools/download_problems.py

(cd specview; make clean ; make)

for f in problems/*.in; do
    id=$(echo $f | sed -E 's@problems/([[:digit:]]+).in@\1@')
    ./specview/specview < problems/$id.in > problems/$id.svg
done

git pull
git add problems/*.in problems/*.svg
git commit -m 'Update problems (by Jenkins)'
git push origin master
