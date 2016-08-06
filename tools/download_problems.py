#!/usr/bin/env python3

from pathlib import Path
import sys
import time

import utils

problem_directory_path = Path(__file__).parents[1] / 'problems'
snapshot = utils.download_latest_snapshot()

for p in snapshot['problems']:
    problem_id = p['problem_id']
    problem_spec_hash = p['problem_spec_hash']
    problem_path = problem_directory_path / '{}.in'.format(problem_id)
    if problem_path.exists() and problem_path.stat().st_size > 0:
        print('problem {} is already exists, skip download'.
              format(problem_id), file=sys.stderr)
        continue

    time.sleep(1.5)
    problem_blob = utils.download_blob(problem_spec_hash)
    with problem_path.open('w') as f:
        f.write(problem_blob.decode('utf-8'))
    print('problem {0} is downloaded, see problems/{0}.in'.
          format(problem_id), file=sys.stderr)

