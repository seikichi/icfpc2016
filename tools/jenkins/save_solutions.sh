#!/bin/bash

set -eux

# make が必要な場合はここに追記
for dir in rectangle multirectangle scoring rotaterectangle; do
    (cd ${dir} ; make);
done

# solver 増やしたらここに追記
for solver in centering rectangle multirectangle rotaterectangle; do
    ./tools/calc_all_resemblances.py ${solver}/${solver} > ${solver}.json
done
