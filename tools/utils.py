#!/usr/bin/env python3

import sys
import json
import gzip
import time
from urllib import request

api_headers = {
  'Expect': '',
  'Accept-Encoding': 'gzip',
  'X-API-Key': '78-2c034ce372dffce9cf3297143c1f8e70',
}

def download_latest_snapshot():
    snapshot_list = download_snapshot_list()
    time.sleep(1.5)
    snapshot_list['snapshots'].sort(key=lambda s: s['snapshot_time'], reverse=True)
    latest_snapshot_hash = snapshot_list['snapshots'][0]['snapshot_hash']
    return json.loads(download_blob(latest_snapshot_hash).decode('utf8'))

def download_snapshot_list():
    url = 'http://2016sv.icfpcontest.org/api/snapshot/list'
    req = request.Request(url, None, api_headers)

    with request.urlopen(req) as response:
        return json.loads(gzip.decompress(response.read()).decode('utf8'))

def download_blob(hash):
    blob_url = 'http://2016sv.icfpcontest.org/api/blob/{}'.format(hash)
    req = request.Request(blob_url, None, api_headers)
    with request.urlopen(req) as response:
        return gzip.decompress(response.read())

def load_best_solutions(*path_list):
    results = []
    for path in sys.argv[1:]:
        with open(path) as f:
            results.append(json.load(f))

    problem_ids = set()
    for r in results:
        for pid in r['problems'].keys():
            problem_ids.add(pid)

    merged = {}
    for pid in problem_ids:
        solution = None
        for r in results:
            if pid not in r['problems']:
                continue
            resemblance = r['problems'][pid]['resemblance']
            if solution is None or solution['resemblance'] < resemblance:
                solution = r['problems'][pid]
        merged[pid] = solution
    return merged
