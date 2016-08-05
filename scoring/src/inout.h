#pragma once

#include "geometry.h"
#include <gmpxx.h>

std::string GetDigits(FILE *file);
mpq_class ReadFrac(FILE *file);
Point ReadPoint(FILE *file);
