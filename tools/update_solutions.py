#!/usr/bin/env python3

import json
import multiprocessing
import os.path
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import utils

def solve(arg):
    index, input_path, n_total, solver_path, score_path = arg
    problem_id = int(re.sub('\.in$', '', input_path.name))

    print('Processing problem_id = {} ({}, {} / {})'.
          format(problem_id, solver_path, index + 1, n_total),
          file=sys.stderr, flush=True)

    resemblance = 0.0
    solution = None

    with tempfile.NamedTemporaryFile() as temp:
        try:
            command = 'timeout 60 {} < {}'.format(solver_path, input_path)
            solution = subprocess.check_output(command, shell=True)
            temp.write(solution)
            temp.flush()

            command = 'timeout 60 {} {} {}'.format(score_path, input_path, temp.name)
            output = subprocess.check_output(command, shell=True)
            resemblance = float(output)
        except:
            t, m, s = sys.exc_info()
            print('Unexpected error: problem_id={}, {}, {}'.format(problem_id, t, m),
                  file=sys.stderr, flush=True)

    return problem_id, resemblance, solution

def list_target_problem_id_list(solutions, solver_hash):
    project_root_path = Path(__file__).parents[1]
    problems_path = project_root_path / 'problems'
    problem_path_list = list(problems_path.glob('*.in'))

    target_problem_id_list = []

    for p in problem_path_list:
        problem_id = re.sub('\.in$', '', p.name)

        # 過去の結果がない or 過去に計算してない なら計算対象
        if not solutions or (problem_id not in solutions['problems']):
            target_problem_id_list.append(problem_id)
            continue

        # 過去に計算していて 1.0 になった or ソルバが変わってないなら除外
        result = solutions['problems'][problem_id]
        if abs(result['resemblance'] - 1.0) < 1e-10 or solver_hash == result['solver_hash']:
            continue

        target_problem_id_list.append(problem_id)

    return target_problem_id_list


def main():
    if len(sys.argv) != 3:
        print('Usage: {} solver_name solution.json'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    solver_name = sys.argv[1]
    solution_path = Path(sys.argv[2])

    solver_config = utils.load_solver_config(solver_name)

    current_solution = None
    if solution_path.exists():
        with solution_path.open() as f:
            current_solution = json.load(f)

    utils.setup_solver(solver_config)

    solver_hash = utils.calc_solver_hash(solver_config)
    target_problem_id_list = list_target_problem_id_list(current_solution, solver_hash)

    solver_path = Path(solver_config['command'])
    project_path = Path(os.path.abspath(__file__)).parents[1]
    problems_path = project_path / 'problems'
    score_path = project_path / 'scoring/scoring'

    problems = {}

    with multiprocessing.Pool() as pool:
        args = []
        for i, pid in enumerate(target_problem_id_list):
            p = problems_path / '{}.in'.format(pid)
            args.append((i, p, len(target_problem_id_list), solver_path, score_path))

        results = pool.map(solve, args)

        for problem_id, resemblance, solution in results:
            if solution is not None:
                solution = solution.decode('utf-8')
            problems[problem_id] = {
                'resemblance': resemblance,
                'solution': solution,
                'solver_hash': solver_hash,
                'solver_name': solver_name,
            }

    if current_solution is not None:
        current_problems = current_solution['problems']
        for problem_id, result in problems.items():
            current_problems[problem_id] = result
    else:
        current_problems = problems

    with solution_path.open('w') as f:
        json.dump({
            'problems': current_problems,
            'solver_path': str(solver_path),
            'timestamp': time.time(),
        }, f)

if __name__ == '__main__':
    main()
