#!/bin/bash

set -eux

(cd scoring ; make)

for solver in centering rectangle rotaterectangle; do
    ./tools/update_solutions.py "${solver}" "${solver}.json"
done
