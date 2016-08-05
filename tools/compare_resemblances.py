#!/usr/bin/env python3

import sys
import json

if len(sys.argv) != 3:
    print('Usage: {} baseline.json target.json'.format(sys.argv[0]),
          file=sys.stderr)
    sys.exit(1)

baseline_path = sys.argv[1]
target_path = sys.argv[2]

with open(baseline_path) as f:
    baseline = json.load(f)

with open(target_path) as f:
    target = json.load(f)

wins = 0
lose_problem_ids = []
total = len(target['problems'].keys())

for problem_id in target['problems'].keys():
    target_resemblance = target['problems'][problem_id]['resemblance']
    baseline_resemblance = baseline['problems'][problem_id]['resemblance']

    if baseline_resemblance <= target_resemblance:
        wins += 1
        continue

    lose_problem_ids.append(problem_id)

lose_problem_ids.sort(key=lambda s: int(s))

print('{} wins {} / {} problems to {}'.
      format(target_path, wins, total, baseline_path))

if len(lose_problem_ids) > 0:
    print()
    print('{} loses following problems:'.format(target_path))
    for problem_id in lose_problem_ids:
        target_resemblance = target['problems'][problem_id]['resemblance']
        baseline_resemblance = baseline['problems'][problem_id]['resemblance']
        print('problem {}: {} resemblance = {} but {} resemblance = {}'.
              format(problem_id, target_path, target_resemblance, baseline_path, baseline_resemblance))
