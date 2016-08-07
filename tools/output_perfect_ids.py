#!/usr/bin/env python3

import sys
import json

def main():
    if len(sys.argv) != 2:
        print('Usage: {} solution.json'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1]) as f:
        solutions = json.load(f)

    problems = solutions['problems']
    perfect_problem_ids = [int(pid) for (pid, r) in problems.items() if abs(r['resemblance'] - 1.0) < 1e-9]
    print(json.dumps(perfect_problem_ids))

if __name__ == '__main__':
    main()

