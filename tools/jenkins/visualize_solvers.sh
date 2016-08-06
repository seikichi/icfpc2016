#!/bin/bash

set -eu

echo 'centering,rectangle,multirectangle,rotaterectangle,all' > total_resemblances.csv

for solver in centering rectangle multirectangle rotaterectangle; do
    total=$(./tools/show_resemblances.py ${solver}.json | cut -d' ' -f3 | awk '{total = total + $1} END{print total}')
    printf "%s," $total >> total_resemblances.csv
done

./tools/show_resemblances.py {centering,rectangle,multirectangle,rotaterectangle}.json | \
    cut -d' ' -f3 | \
    awk '{total = total + $1} END{print total}' >> total_resemblances.csv

# count cleared (= resemblance = 1.0) problems 
echo 'centering,rectangle,multirectangle,rotaterectangle,all' > completed_problems.csv

for solver in centering rectangle multirectangle rotaterectangle; do
    total=$(./tools/show_resemblances.py ${solver}.json | grep -E ' 1(\.0*)?$' | wc -l)
    printf "%s," $total >> completed_problems.csv
done

./tools/show_resemblances.py {centering,rectangle,multirectangle,rotaterectangle}.json | \
    grep -E ' 1(\.0*)?$' | wc -l >> completed_problems.csv
