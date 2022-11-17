#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

int sum_n(int n) {
  if (n < 0) {
    return -1;
  }
  return n * (n + 1) / 2;
}

TEST_CASE("Testing the sum_n function") {
  CHECK(sum_n(10) == 55);
  CHECK(sum_n(9) == 45);
  CHECK(sum_n(0) == 0);
  CHECK(sum_n(-4) == -1);
  CHECK(sum_n(-5) == -1);
}
