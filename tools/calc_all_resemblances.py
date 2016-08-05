#!/usr/bin/env python3

import json
import os
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path

project_path = Path(__file__).parents[1]
problems_path = project_path / 'problems'
globs = list(problems_path.glob('*.in'))

SCORE_COMMAND = project_path / 'scoring/scoring'
SOLVER_COMMAND = os.environ.get('SOLVER')

problems = {}
for i, p in enumerate(globs):
    problem_id = int(re.sub('\.in$', '', p.name))

    print('Processing problem_id = {} ({} / {})'.
          format(problem_id, i + 1, len(globs)), file=sys.stderr)

    resemblance = float('nan')
    solution = None

    with tempfile.NamedTemporaryFile() as temp:
        try:
            command = '{} < {}'.format(SOLVER_COMMAND, p)
            solution = subprocess.check_output(command, shell=True)
            temp.write(solution)
            temp.flush()

            command = '{} {} {}'.format(SCORE_COMMAND, p, temp.name)
            output = subprocess.check_output(command, shell=True)
            resemblance = float(output)
        except:
            print('Unexpected error:', sys.exc_info()[0])

    problems[problem_id] = {
        'resemblance': resemblance,
        'solution': solution.decode('utf-8'),

    }

print(json.dumps({
    'problems': problems,
    'solver_command': SOLVER_COMMAND,
    'timestamp': time.time(),
}))
