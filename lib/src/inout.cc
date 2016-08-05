#include "inout.h"

#include <assert.h>
#include <gmpxx.h>

std::string GetDigits(FILE *file) {
  std::string ret;
  fscanf(file, " ");
  while (true) {
    int c = getc(file);
    assert(c >= 0);
    if (!isdigit(c) && c != '-') {
      ungetc(c, file);
      break;
    }
    ret += c;
  }
  return ret;
}
mpq_class ReadFrac(FILE *file) {
  std::string a = GetDigits(file);
  std::string b = "1";
  assert(a.size() > 0);
  int c = getc(file);
  assert(c >= 0);
  if (c != '/') {
    ungetc(c, file);
  } else {
    b = GetDigits(file);
    assert(b.size() > 0);
  }
  mpz_class num(a);
  mpz_class den(b);
  mpq_class q(num, den);
  return q;
}
Point ReadPoint(FILE *file) {
  mpq_class x = ReadFrac(file);
  int v = fscanf(file, " ,");
  assert(v == 0);
  mpq_class y = ReadFrac(file);
  return Point(x, y);
}
