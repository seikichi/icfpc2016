#!/usr/bin/env python3

import json
import time

import utils

snapshot_list = utils.download_snapshot_list()
time.sleep(1.5)
snapshot_list['snapshots'].sort(key=lambda s: s['snapshot_time'], reverse=True)
latest_snapshot_hash = snapshot_list['snapshots'][0]['snapshot_hash']

snapshot = json.loads(utils.download_blob(latest_snapshot_hash).decode('utf8'))

for p in snapshot['problems']:
    problem_id = p['problem_id']
    resemblances = map(lambda r: str(r['resemblance']), p['ranking'][:5])
    print('problem {}: {} ...'.format(problem_id, ', '.join(resemblances)))
