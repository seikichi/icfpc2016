#include "crease.h"
#include <iostream>
using namespace std;

vector<pair<mpq_class, mpq_class>>
CalculateCreasesInternal(mpq_class length, const mpq_class& target) {
  if (target >= length) {
    return {};
  } else if (target >= length/2) {
    return {make_pair(target, target)};
  } else {
    auto creases = CalculateCreasesInternal(length/2, target);
    creases.reserve(creases.size()*2 + 1);
    if (creases.size() == 1) {
      creases.push_back(make_pair(length/2, 2*target-length/2));
    } else {
      creases.push_back(make_pair(length/2, 0));
    }
    for (int i = creases.size()-2; i >= 0; --i) {
      creases.push_back(make_pair(length - creases[i].first, creases[i].second));
    }
    return creases;
  }
}


vector<pair<mpq_class, mpq_class>>
CalculateCreases(mpq_class length, const mpq_class& target) {
  auto result = CalculateCreasesInternal(length, target);
  if (result.size() == 0) {
    result.push_back(make_pair(1, 1));
  } else if (result.size() == 1) {
    result.push_back(make_pair(1, 2*target-length));
  } else {
    result.push_back(make_pair(1, 0));
  }
  result.insert(result.begin(), make_pair(0, 0));
  return result;
}
