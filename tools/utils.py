#!/usr/bin/env python3

import sys
import json

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
