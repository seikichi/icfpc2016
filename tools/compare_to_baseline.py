#!/usr/bin/env python3

import sys
import json
import math

BASELINE = 'centering'

if len(sys.argv) != 3:
    print('Usage: {} baseline.json target.json'.format(sys.argv[0]),
          file=sys.stderr)
    sys.exit(1)

with open(sys.argv[1]) as f:
    baseline = json.load(f)

with open(sys.argv[2]) as f:
    target = json.load(f)

target_lose_to_baseline = False

for problem_id in target['problems'].keys():
    target_resemblance = target['problems'][problem_id]['resemblance']
    baseline_resemblance = baseline['problems'][problem_id]['resemblance']

    if baseline_resemblance <= target_resemblance:
        continue

    target_lose_to_baseline = True
    print('problem {}: target resemblance = {} but base resemblance = {}'.
          format(problem_id, target_resemblance, baseline_resemblance),
          file=sys.stderr)

if target_lose_to_baseline:
    sys.exit(1)
