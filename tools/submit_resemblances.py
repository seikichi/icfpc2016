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
        result = utils.submit_solution(problem_id, problem_result['solution'])
        print(json.dumps(result), file=sys.stderr)

        if result['ok'] and abs(problem_result['resemblance'] - result['resemblance']) > 0.1:
            print('resemblance may be wrong: our {}, but got {}'.
                  format(problem_result['resemblance'], result['resemblance']), file=sys.stderr)

if __name__ == '__main__':
    main()
