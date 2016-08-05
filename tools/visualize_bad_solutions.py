#!/usr/bin/env python3

import sys
import json
import utils
import tempfile
import subprocess
import os

if len(sys.argv) < 2:
    print('Usage: {} result1.json result2.json...'.format(sys.argv[0]),
          file=sys.stderr)
    sys.exit(1)

solvers = []
for path in sys.argv[1:]:
    with open(path) as f:
        solvers.append(json.load(f)['solver_path'])

solution_list = []
for pid, s in utils.load_solutions(sys.argv[1:]).items():
    s['problem_id'] = pid
    solution_list.append(s)

solution_list.sort(key=lambda s: s['resemblance'])
bad_solutions = solution_list[:10]

mergeview_path = 'mergeview/mergeview'

if not os.path.exists('bad_solutions'):
    os.makedirs('bad_solutions')

problem_id_list = []
resemblance_list = []

for s in bad_solutions:
    problem_id_list.append(s['problem_id'])
    resemblance_list.append(s['resemblance'])

    # for solver in solvers:
    #     input_path = 'problems/{}.in'.format(s['problem_id'])
    #     with tempfile.NamedTemporaryFile() as temp:
    #         try:
    #             command = '{} < {}'.format(solver, input_path)
    #             solution = subprocess.check_output(command, shell=True)
    #             temp.write(solution)
    #             temp.flush()

    #             output_path = 'bad_solutions/{}.{}.merge.svg'.format(s['problem_id'], solver.split('/')[-1])
    #             command = '{} {} {} > {}'.format(mergeview_path, input_path, temp.name, output_path)
    #             output = subprocess.check_output(command, shell=True)
    #         except:
    #             print('Unexpected error:', sys.exc_info()[0], file=sys.stderr)

solver_names = list(map(lambda s: s.split('/')[-1], solvers))

with open('bad_solutions/index.html', 'w') as f:
    f.write('''
<!DOCTYPE html>
<html lang="en">
  <head>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css" integrity="sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp" crossorigin="anonymous">
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script></head>
  <body>
    ''')

    for i in range(len(problem_id_list)):
        problem_id = problem_id_list[i]
        resemblance = resemblance_list[i]

        f.write('<div class="row">')
        f.write('<p>problem {}: resemblance = {}</p>'.format(problem_id, resemblance))

        for s in solver_names:
            path = '{}.{}.merge.svg'.format(problem_id, s)
            f.write('<div class="col-md-1">')
            f.write('<object data="{}" type="image/svg+xml" width="200" height="200"></object>'.format(path))
            f.write('</div>')

        f.write('</div>')

    f.write('''
  </body>
</html>
    ''')
