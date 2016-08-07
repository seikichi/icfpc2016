#!/usr/bin/env python3

import sys
import json
import time
from pathlib import Path

import utils

def main():
    if len(sys.argv) <= 1:
        print('Usage: {} 1.json 2.json ...'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    solutions = []
    for path in map(Path, sys.argv[1:]):
        with path.open() as f:
            solutions.append(json.load(f))

    problem_ids = set()
    for s in solutions:
        for pid in s['problems'].keys():
            problem_ids.add(pid)

    problems = {}
    for pid in problem_ids:
        result = None
        for s in solutions:
            if pid not in s['problems']:
                continue
            if (result is None or
                result['resemblance'] < s['problems'][pid]['resemblance']):
                result = s['problems'][pid]
        assert(result is not None)
        problems[pid] = result

    print(json.dumps({
        'problems': problems,
        'timestamp': time.time(),
    }))

if __name__ == '__main__':
    main()
