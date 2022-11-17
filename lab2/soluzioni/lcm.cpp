#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

int lcm(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  }

  const int step{a > b ? std::abs(a) : std::abs(b)};
  int result{step};

  while (!(result % a == 0 && result % b == 0)) {
    result += step;
  }

  return result;
}

TEST_CASE("testing the lcm function") {
  CHECK(lcm(12, 16) == 48);
  CHECK(lcm(20, 25) == 100);
  CHECK(lcm(0, 1) == 0);
  CHECK(lcm(1, 0) == 0);
  CHECK(lcm(0, 0) == 0);
  CHECK(lcm(12, -16) == 48);
  CHECK(lcm(-12, 16) == 48);
  CHECK(lcm(-12, -16) == 48);
}
