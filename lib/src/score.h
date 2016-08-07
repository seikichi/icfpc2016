#pragma once
#include "in.h"
#include "out.h"

double ScoringMonte(const char *input_filename, const char *solution_filename, int cnt);
double ScoringMonte(const Input &input, const Output &output, int cnt);

// bool PerfectHit(const char *input_filename, const char *solution_filename, int cnt);
// bool PerfectHit(const Input &input, const Output &ouput, int cnt);
