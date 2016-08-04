#include <iostream>
#include <memory>
#include "answer.h"

int main() {
  auto p = std::make_unique<Answer>();
  std::cout << "the Answer to the Ultimate Question "
               "of Life, the Universe, and Everything = "
            << p->UltimateQuestion() << std::endl;
  return 0;
}
