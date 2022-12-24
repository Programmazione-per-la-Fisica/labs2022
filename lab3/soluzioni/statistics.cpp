#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cmath>

#include "doctest.h"

struct Statistics {
  double mean{};
  double sigma{};
  double mean_err{};
};

class Sample {
  double sum_x_{};
  double sum_x2_{};
  int N_{};

 public:
  int size() const { return N_; }

  void add(double x) {
    sum_x_ += x;
    sum_x2_ += x * x;
    ++N_;
  }

  Statistics statistics() const {
    if (N_ < 2) {
      throw std::runtime_error{"Not enough points to run an statistics"};
    }

    const double mean = sum_x_ / N_;
    const double sigma = std::sqrt((sum_x2_ - N_ * mean * mean) / (N_ - 1));
    const double mean_err = sigma / std::sqrt(N_);

    return {mean, sigma, mean_err};
  }
};

TEST_CASE("Testing the class handling a floating point data sample") {
  Sample sample;

  REQUIRE(sample.size() == 0);

  SUBCASE("Calling size() with four points") {
    sample.add(1.0);
    sample.add(1.5);
    sample.add(2.0);
    sample.add(2.5);
    CHECK(sample.size() == 4);
  }

  SUBCASE("Calling statistics() with no points throws") {
    CHECK_THROWS(sample.statistics());
  }

  SUBCASE("Calling statistics() with one point throws") {
    sample.add(4.0);
    CHECK_THROWS(sample.statistics());
  }

  SUBCASE("Calling statistics() with two points") {
    sample.add(1.);
    sample.add(2.);
    auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(1.5));
    CHECK(result.sigma == doctest::Approx(0.70710678));
    CHECK(result.mean_err == doctest::Approx(0.5));
  }

  SUBCASE("Calling statistics() with two identical points") {
    sample.add(10.);
    sample.add(10.);
    auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(10.0));
    CHECK(result.sigma == doctest::Approx(0.0));
    CHECK(result.mean_err == doctest::Approx(0.0));
  }

  SUBCASE("Calling statistics() with five points") {
    sample.add(1.5);
    sample.add(2.0);
    sample.add(2.5);
    sample.add(3.0);
    sample.add(3.5);
    auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(2.5));
    CHECK(result.sigma == doctest::Approx(0.79056942));
    CHECK(result.mean_err == doctest::Approx(0.35355339));
  }
}