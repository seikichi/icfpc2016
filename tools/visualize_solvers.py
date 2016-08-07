#!/usr/bin/env python3

import json
from pathlib import Path

import utils

def output_perfect_problems(results):
    perfect = {}
    for name in results.keys():
        problems = results[name]['problems']
        perfect[name] = sum(1 for p in problems.values() if abs(p['resemblance'] - 1.0) < 1e-9)

    with open('perfect.csv', 'w') as f:
        keys = list(sorted(results.keys()))
        f.write('{}\n'.format(','.join(keys)))
        f.write('{}\n'.format(','.join(str(perfect[k]) for k in keys)))

def output_average_resemblance(results):
    averages = {}
    for name in results.keys():
        problems = results[name]['problems']
        averages[name] = sum(p['resemblance'] for p in problems.values()) / len(problems)

    with open('average.csv', 'w') as f:
        keys = list(sorted(results.keys()))
        f.write('{}\n'.format(','.join(keys)))
        f.write('{}\n'.format(','.join(str(averages[k]) for k in keys)))

def output_median_resemblance(results):
    medians = {}
    for name in results.keys():
        problems = results[name]['problems']
        medians[name] = sorted(p['resemblance'] for p in problems.values())[len(problems) // 2]

    with open('median.csv', 'w') as f:
        keys = list(sorted(results.keys()))
        f.write('{}\n'.format(','.join(keys)))
        f.write('{}\n'.format(','.join(str(medians[k]) for k in keys)))

def output_total_resemblance(results):
    totals = {}
    for name in results.keys():
        problems = results[name]['problems']
        totals[name] = sum(p['resemblance'] for p in problems.values())

    with open('total.csv', 'w') as f:
        keys = list(sorted(results.keys()))
        f.write('{}\n'.format(','.join(keys)))
        f.write('{}\n'.format(','.join(str(totals[k]) for k in keys)))

def main():
    configs = utils.load_all_solvers_config()

    results = {}
    for name in list(map(lambda c: c['name'], configs)) + ['solutions']:
        json_path = Path('{}.json'.format(name))
        if not json_path.exists():
            continue
        with json_path.open() as f:
            results[name] = json.load(f)

    output_perfect_problems(results)
    output_average_resemblance(results)
    output_median_resemblance(results)
    output_total_resemblance(results)

if __name__ == '__main__':
    main()

