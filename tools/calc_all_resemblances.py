#!/usr/bin/env python3

import json
import multiprocessing
import os.path
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path


def solve(arg):
    index, input_path, n_total, solver_path, score_path = arg
    problem_id = int(re.sub('\.in$', '', input_path.name))

    print('Processing problem_id = {} ({}, {} / {})'.
          format(problem_id, solver_path, index + 1, n_total), file=sys.stderr)

    resemblance = float('nan')
    solution = None

    with tempfile.NamedTemporaryFile() as temp:
        try:
            command = '{} < {}'.format(solver_path, input_path)
            solution = subprocess.check_output(command, shell=True)
            temp.write(solution)
            temp.flush()

            command = '{} {} {}'.format(score_path, input_path, temp.name)
            output = subprocess.check_output(command, shell=True)
            resemblance = float(output)
        except:
            print('Unexpected error:', sys.exc_info()[0], file=sys.stderr)

    return problem_id, resemblance, solution


def main():
    if len(sys.argv) != 2:
        print('Usage: {} solver_path'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    solver_path = sys.argv[1]
    project_path = Path(os.path.abspath(__file__)).parents[1]
    problems_path = project_path / 'problems'
    score_path = project_path / 'scoring/scoring'

    problems = {}

    with multiprocessing.Pool() as pool:
        globs = list(problems_path.glob('*.in'))
        args = [(i, p, len(globs), solver_path, score_path)
                for i, p in enumerate(globs)]
        results = pool.map(solve, args)

        for problem_id, resemblance, solution in results:
            problems[problem_id] = {
                'resemblance': resemblance,
                'solution': solution.decode('utf-8'),
            }

    print(json.dumps({
        'problems': problems,
        'solver_path': solver_path,
        'timestamp': time.time(),
    }))


if __name__ == '__main__':
    main()
