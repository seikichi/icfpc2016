#!/usr/bin/env python3

import sys
import json

import utils

if len(sys.argv) < 2:
    print('Usage: {} result.json ...'.format(sys.argv[0]), file=sys.stderr)
    sys.exit(1)

solutions = utils.load_best_solutions(sys.argv[1:])
complete_problem_count = 0

for pid in solutions.keys():
    resemblance = solutions[pid]['resemblance']
    if abs(resemblance - 1.0) < 1e-7:
        print('problem {}: resemblance is {}'.
              format(pid, resemblance),
              file=sys.stderr)
        complete_problem_count += 1

print(complete_problem_count)
