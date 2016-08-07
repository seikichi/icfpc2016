#!/bin/bash

set -eux

(cd scoring ; make clean ; make)
curl 'http://104.199.162.10/perfect_problem_ids' > perfect_problem_ids
./tools/update_solutions.py "${1}" "${1}.json" perfect_problem_ids
