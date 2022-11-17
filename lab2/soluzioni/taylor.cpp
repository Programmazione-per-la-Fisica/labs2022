#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cmath>

#include "doctest.h"

int factorial(int n) {
  if (n < 0) {
    return -1;
  } else if (n == 0 || n == 1) {
    return 1;
  }

  return n * factorial(n - 1);
}

double taylor(double x, int N) {
  if (N < 0) {
    return -1.0;
  }

  double sum{};
  for (int n{}; n <= N; ++n) {
    const int nt2{n * 2};
    sum += std::pow(-1, n) * std::pow(x, nt2) / factorial(nt2);
  }

  return sum;
}

TEST_CASE("Testing the factorial function") {
  CHECK(factorial(-10) == -1);
  CHECK(factorial(1) == 1);
  CHECK(factorial(3) == 6);
  CHECK(factorial(10) == 3628800);
}

TEST_CASE("Testing the taylor expansion function") {
  CHECK(taylor(0.5, 0) == doctest::Approx(1.0));
  CHECK(taylor(0.5, 1) == doctest::Approx(0.875));
  CHECK(taylor(0.5, 2) == doctest::Approx(0.877).epsilon(0.001));
  CHECK(taylor(0.5, 3) == doctest::Approx(0.877).epsilon(0.001));
  CHECK(taylor(0.5, -9) == doctest::Approx(-1.0));
}
