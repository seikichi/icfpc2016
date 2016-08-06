#!/usr/bin/env python3

import sys
import json
import time
import utils

def main():
    if len(sys.argv) != 2:
        print('Usage: {} result.json'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1]) as f:
        result = json.load(f)

    total = len(result['problems'].items())
    for problem_id, problem_result in result['problems'].items():
        time.sleep(1.5)
        print('submit problem {}, resemblance {}'.
              format(problem_id, problem_result['resemblance']), file=sys.stderr)
        print(utils.submit_solution(problem_id, problem_result['solution']).decode('utf-8'), file=sys.stderr)

if __name__ == '__main__':
    main()
