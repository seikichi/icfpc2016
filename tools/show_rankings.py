#!/usr/bin/env python3

import utils

snapshot = utils.download_latest_snapshot()

for p in snapshot['problems']:
    problem_id = p['problem_id']
    resemblances = map(lambda r: str(r['resemblance']), p['ranking'][:5])
    print('problem {}: {} ...'.format(problem_id, ' '.join(resemblances)))
