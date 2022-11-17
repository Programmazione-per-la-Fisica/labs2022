#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cmath>

#include "doctest.h"

double taylor_opt(double x, int N) {
  if (N < 0) {
    return -1.0;
  }

  double sum{};
  int fact{1};

  for (int n{}; n <= N; ++n) {
    const int nt2{n * 2};
    for (int i{n + 1}; i <= nt2; ++i) {
      fact *= i;
    }
    sum += std::pow(-1, n) * std::pow(x, nt2) / fact;
  }

  return sum;
}

TEST_CASE("Testing the optimized taylor expansion function") {
  CHECK(taylor_opt(0.5, 0) == doctest::Approx(1.0));
  CHECK(taylor_opt(0.5, 1) == doctest::Approx(0.875));
  CHECK(taylor_opt(0.5, 2) == doctest::Approx(0.877).epsilon(0.001));
  CHECK(taylor_opt(0.5, 3) == doctest::Approx(0.877).epsilon(0.001));
  CHECK(taylor_opt(0.5, -9) == doctest::Approx(-1.0));
}
