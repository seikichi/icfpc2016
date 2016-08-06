#!/bin/bash

set -eux

./tools/update_solutions.py "${1}" "${1}.json"
