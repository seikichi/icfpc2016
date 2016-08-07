#!/bin/bash

set -eu

JOB_ID=$1
PROBLEM_ID=$2
SOLVER=$3

curl -s -u "icfpc2016:kmckmckmc" \
     "http://104.199.162.10/job/submit-updated-solutions/${JOB_ID}/artifact/solvers/${SOLVER}.json" | \
    jq ".problems[\"${PROBLEM_ID}\"]"
