#!/usr/bin/env python3

import sys

import utils

if len(sys.argv) < 2:
    print('Usage: {} result1.json result2.json...'.format(sys.argv[0]),
          file=sys.stderr)
    sys.exit(1)

solutions = utils.load_solutions(sys.argv[1:])
for problem_id in sorted(solutions.keys(), key=lambda s: int(s)):
    print('problem {}: {}'.format(problem_id,
                                  solutions[problem_id]['resemblance']))
