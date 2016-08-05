#pragma once
#include <vector>
#include <utility>
#include <gmpxx.h>

std::vector<std::pair<mpq_class, mpq_class>>
CalculateCreases(mpq_class length, const mpq_class& target);
