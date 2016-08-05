#!/bin/bash

set -eux

# make が必要な場合はここに追記
for dir in rectangle multirectangle scoring; do
    (cd ${dir} ; make);
done

timestamp=$(date +%s)

# solver 増やしたらここに追記
for solver in centering rectangle multirectangle; do
    mkdir -p ${DATA_DIR}/${solver}
    ./tools/calc_all_resemblances.py ${solver}/${solver} > ${DATA_DIR}/${solver}/${timestamp}.json
    (cd ${DATA_DIR}/${solver} ; ln -fs "${timestamp}.json" "latest.json")
done
