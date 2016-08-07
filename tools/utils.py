#!/usr/bin/env python3

import yaml
import gzip
import json
import subprocess
import sys
import tempfile
import time
from pathlib import Path
from urllib import request
from urllib.error import HTTPError
from urllib.parse import urlencode

api_headers = {
  'Expect': '',
  'Accept-Encoding': 'gzip',
  'X-API-Key': '78-2c034ce372dffce9cf3297143c1f8e70',
}

def submit_solution(problem_id, solution):
    url = 'http://2016sv.icfpcontest.org/api/solution/submit'
    values = {'problem_id': problem_id, 'solution_spec': solution}
    data = urlencode(values).encode('ascii')
    req = request.Request(url, data, api_headers)
    return json.loads(_request_with_retry(req).decode('utf8'))

def download_latest_snapshot():
    snapshot_list = download_snapshot_list()
    time.sleep(1.5)
    snapshot_list['snapshots'].sort(key=lambda s: s['snapshot_time'], reverse=True)
    latest_snapshot_hash = snapshot_list['snapshots'][0]['snapshot_hash']
    return json.loads(download_blob(latest_snapshot_hash).decode('utf8'))

def download_snapshot_list():
    url = 'http://2016sv.icfpcontest.org/api/snapshot/list'
    req = request.Request(url, None, api_headers)
    return json.loads(_request_with_retry(req).decode('utf8'))

def download_blob(hash):
    blob_url = 'http://2016sv.icfpcontest.org/api/blob/{}'.format(hash)
    req = request.Request(blob_url, None, api_headers)
    return _request_with_retry(req)

def _request_with_retry(req):
    retry = 5

    for i in range(retry):
        try:
            with request.urlopen(req) as response:
                return gzip.decompress(response.read())
        except HTTPError as e:
            if e.code == 403:
                raise e
            if e.code == 429:
                print('Got too many request error, wait 5 seconds...',
                      file=sys.stderr)
                time.sleep(5)
                continue
            print('HTTP Error: {}'.format(e.read().decode('utf-8')),
                  file=sys.stderr)
            if i >= retry:
                raise e

    raise RuntimeError('Failed to use ICFPC API ... ({} times retried)'.format(retry))

def load_solutions(*path_list):
    solutions = []
    for path in sys.argv[1:]:
        with open(path) as f:
            solutions.append(json.load(f))

    problem_ids = set()
    for s in solutions:
        for pid in s['problems'].keys():
            problem_ids.add(pid)

    merged_solution = {}
    for pid in problem_ids:
        solution = None
        for s in solutions:
            if pid not in s['problems']:
                continue
            resemblance = s['problems'][pid]['resemblance']
            if solution is None or solution['resemblance'] < resemblance:
                solution = s['problems'][pid]
                solution['solver_path'] = s['solver_path']
        merged_solution[pid] = solution
    return merged_solution

def load_all_solvers_config():
    project_root_path = Path(__file__).parents[1]
    solver_yml_path = project_root_path / 'solvers.yml'

    with solver_yml_path.open() as f:
        return yaml.load(f)

def load_solver_config(solver_name):
    all_config = load_all_solvers_config()
    assert(solver_name in map(lambda c: c['name'], all_config))
    return next(c for c in all_config if c['name'] == solver_name)

def setup_solver(config):
    setup = config.get('setup', None)
    if setup is None:
        return
    subprocess.check_output(setup, shell=True)

def calc_solver_hash(config):
    command = 'git log --pretty=format:"%H" -n 1 {}'.format(' '.join(config['dependent']))
    return subprocess.check_output(command, shell=True).decode('utf8')
