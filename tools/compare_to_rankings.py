#!/usr/bin/env python3

import sys

import utils

if len(sys.argv) < 2:
    print('Usage: {} result1.json result2.json...'.format(sys.argv[0]),
          file=sys.stderr)
    sys.exit(1)

solutions = utils.load_solutions(sys.argv[1:])
snapshot = utils.download_latest_snapshot()

for p in snapshot['problems']:
    problem_id = p['problem_id']
    ranking_top_resemblance = p['ranking'][0]['resemblance']
    our_resemblance = solutions[str(problem_id)]['resemblance']
    our_resemblance = solutions[str(problem_id)]['resemblance']

    print('problem {}: top_resemblance-our_resemblance= {} (top = {}, our = {})'.
          format(problem_id,
                 ranking_top_resemblance - our_resemblance,
                 ranking_top_resemblance,
                 our_resemblance))
