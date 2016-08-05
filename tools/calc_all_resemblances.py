#!/usr/bin/env python3

import json
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path

if len(sys.argv) != 2:
    print('Usage: {} solver_path'.format(sys.argv[0]), file=sys.stderr)
    sys.exit(1)

solver_path = sys.argv[1]
project_path = Path(__file__).parents[1]
problems_path = project_path / 'problems'
score_path = project_path / 'scoring/scoring'

problems = {}
globs = list(problems_path.glob('*.in'))
for i, p in enumerate(globs):
    problem_id = int(re.sub('\.in$', '', p.name))

    print('Processing problem_id = {} ({}, {} / {})'.
          format(problem_id, solver_path, i + 1, len(globs)), file=sys.stderr)

    resemblance = float('nan')
    solution = None

    with tempfile.NamedTemporaryFile() as temp:
        try:
            command = '{} < {}'.format(solver_path, p)
            solution = subprocess.check_output(command, shell=True)
            temp.write(solution)
            temp.flush()

            command = '{} {} {}'.format(score_path, p, temp.name)
            output = subprocess.check_output(command, shell=True)
            resemblance = float(output)
        except:
            print('Unexpected error:', sys.exc_info()[0], file=sys.stderr)

    problems[problem_id] = {
        'resemblance': resemblance,
        'solution': solution.decode('utf-8'),
    }

print(json.dumps({
    'problems': problems,
    'solver_path': solver_path,
    'timestamp': time.time(),
}))
