#!/usr/bin/python3
import sys
import os.path as path
import subprocess
import tempfile

problem = sys.stdin.read()
top_dir = path.dirname(path.dirname(path.abspath(__file__)))

max_score = -1.0
best_output = None

for initial in sys.argv[1:]:
    print("Run handbrute {}".format(initial), flush=True, file=sys.stderr)
    p = subprocess.Popen([path.join(top_dir, "handbrute", "handbrute"), initial],
                         stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    hb_stdout, _ = p.communicate(problem.encode('utf-8'))
    if p.returncode != 0:
        print("Error: handbrute {} returns {}".format(initial, p.returncode),
              flush=True, file=sys.stderr)
        continue

    with tempfile.NamedTemporaryFile() as solution_f, \
         tempfile.NamedTemporaryFile() as input_f:
        solution_f.write(hb_stdout)
        solution_f.flush()

        input_f.write(problem.encode('utf-8'))
        input_f.flush()

        p = subprocess.Popen([path.join(top_dir, "scoring", "scoring"), input_f.name, solution_f.name],
                             stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        s_stdout, _ = p.communicate(hb_stdout)
        if p.returncode !=  0:
            print("Error: scoring ({}) returns {}".format(initial, p.returncode),
                  flush=True, file=sys.stderr)
            continue

    score = float(s_stdout)
    if score > max_score:
        max_score = score
        best_output = hb_stdout
        if score == 1.0:
            break

if best_output is None:
    print("Error: nothing to output!!", flush=True, file=sys.stderr)
    sys.exit(1)

print(best_output.decode('utf-8'))
