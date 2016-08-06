#!/usr/bin/python3
import argparse
import os.path as path
import subprocess
import tempfile
import time


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("solver", metavar="SOLVER")
    parser.add_argument("problem", metavar="PROBLEM")
    args = parser.parse_args()

    top_dir = path.dirname(path.dirname(path.abspath(__file__)))
    scoring = path.join(top_dir, 'scoring/scoring')
    mergeview = path.join(top_dir, 'mergeview/mergeview')

    with tempfile.NamedTemporaryFile(suffix='.out', delete=False) as f, \
         tempfile.NamedTemporaryFile(suffix='.svg', delete=False) as svg, \
         open(args.problem) as problem_file:
        subprocess.check_call([args.solver], stdin=problem_file, stdout=f)
        print("Output: {}".format(f.name))
        subprocess.check_call([mergeview, args.problem, f.name], stdout=svg)
        print("SVG: {}".format(svg.name))
        print("Score: ")
        subprocess.check_call([scoring, args.problem, f.name])
        subprocess.check_call(["google-chrome", svg.name])


if __name__ == '__main__':
    main()
