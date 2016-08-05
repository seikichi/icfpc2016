#!/usr/bin/env python3

import json
import gzip
import time
import sys
from urllib import request

import utils

if len(sys.argv) < 2:
    print('Usage: {} result.json ...'.format(sys.argv[0]), file=sys.stderr)
    sys.exit(1)

snapshot_list = utils.download_snapshot_list()
time.sleep(2)
snapshot_list['snapshots'].sort(key=lambda s: s['snapshot_time'], reverse=True)
latest_snapshot_hash = snapshot_list['snapshots'][0]['snapshot_hash']

snapshot = json.loads(utils.download_blob(latest_snapshot_hash).decode('utf8'))

for p in snapshot['problems']:
    problem_id = p['problem_id']
    top_resemblance = p['ranking'][0]['resemblance']
    print(problem_id, top_resemblance)
    # our_resemblance = solutions[str(problem_id)]['resemblance']

    # if top_resemblance - our_resemblance > 0.5:
    #     print('problem {}: ranking top is {}, but our score is {}'.
    #           format(pid, top_resemblance, our_resemblance))
