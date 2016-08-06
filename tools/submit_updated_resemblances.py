#!/usr/bin/env python3

import sys
import json
import time
import utils

def main():
    if len(sys.argv) != 3:
        print('Usage: {} old.json new.json'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1]) as f:
        old_result = json.load(f)

    with open(sys.argv[2]) as f:
        new_result = json.load(f)

    merged = new_result.copy()
    merged_problems = {}

    for problem_id, problem_result in new_result['problems'].items():
        merged_problems[problem_id] = problem_result
        old_problem_result = old_result['problems'].get(problem_id, {})

        if problem_result['solution'] == old_problem_result.get('solution', None):
            print('skip to submit problem {}. solutions are same'.format(problem_id), file=sys.stderr)
            continue

        if problem_result['resemblance'] <= old_problem_result.get('resemblance', -1.0):
            print('skip to submit problem {}. resemblance becomes worse ({} -> {})'.
                  format(problem_id,
                         old_problem_result.get('resemblance', '[not exist]'),
                         problem_result['resemblance']),
                  file=sys.stderr)
            merged_problems[problem_id] = old_problem_result
            continue

        time.sleep(1.5)
        print('submit problem {}, resemblance {} (before {})'.
              format(problem_id,
                     problem_result['resemblance'],
                     old_problem_result.get('resemblance', '[not exist]')),
              file=sys.stderr)
        try:
            result = utils.submit_solution(problem_id, problem_result['solution'])
            print(json.dumps(result), file=sys.stderr)

            if result['ok'] and abs(problem_result['resemblance'] - result['resemblance']) > 0.1:
                print('resemblance may be wrong: our {}, but got {}'.
                      format(problem_result['resemblance'], result['resemblance']), file=sys.stderr)
        except:
            print('Unexpected error: {}'.format(sys.exc_info()[0]))

    merged = new_result.copy()
    merged['problems'] = merged_problems
    print(json.dumps(merged))

if __name__ == '__main__':
    main()
