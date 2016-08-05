#!/usr/bin/env python3

import sys
import json

import utils

if len(sys.argv) < 2:
    print('Usage: {} result.json ...'.format(sys.argv[0]), file=sys.stderr)
    sys.exit(1)

solutions = utils.load_best_solutions(sys.argv[1:])
complete_problem_count = 0

solution_list = []
for pid in solutions.keys():
    s = solutions[pid].copy()
    s.update({'problem_id': pid})
    solution_list.append(s)

solution_list.sort(key=lambda s: s['resemblance'], reverse=False)
for s in solution_list[:10]:
    print('problem {}: resemblance is {}!'.
          format(s['problem_id'], s['resemblance']))
