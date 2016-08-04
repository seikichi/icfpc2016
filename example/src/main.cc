#include <iostream>
#include "answer.h"

int main() {
  std::cout << "the Answer to the Ultimate Question "
               "of Life, the Universe, and Everything = "
            << Answer().UltimateQuestion() << std::endl;
  return 0;
}
