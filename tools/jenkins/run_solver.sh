#!/bin/bash

set -eux

(cd scoring ; make)
./tools/update_solutions.py "${1}" "${1}.json"
