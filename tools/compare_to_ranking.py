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

solutions = utils.load_best_solutions(sys.argv[1:])

url = 'http://2016sv.icfpcontest.org/api/snapshot/list'
headers = {
    'Expect': '',
    'Accept-Encoding': 'gzip',
    'X-API-Key': '78-2c034ce372dffce9cf3297143c1f8e70',
}
req = request.Request(url, None, headers)

with request.urlopen(req) as response:
    snapshot_list = json.loads(gzip.decompress(response.read()).decode('utf8'))
    snapshot_list['snapshots'].sort(key=lambda s: s['snapshot_time'], reverse=True)
    latest_snapshot_hash = snapshot_list['snapshots'][0]['snapshot_hash']

time.sleep(2)
blob_url = 'http://2016sv.icfpcontest.org/api/blob/{}'.format(latest_snapshot_hash)
req = request.Request(blob_url, None, headers)
with request.urlopen(req) as response:
    data = json.loads(gzip.decompress(response.read()).decode('utf8'))

for p in data['problems']:
    pid = p['problem_id']
    top_resemblance = p['ranking'][0]['resemblance']
    our_resemblance = solutions[str(pid)]['resemblance']

    if top_resemblance - our_resemblance > 0.5:
        print('problem {}: ranking top is {}, but our score is {}'.
              format(pid, top_resemblance, our_resemblance))
