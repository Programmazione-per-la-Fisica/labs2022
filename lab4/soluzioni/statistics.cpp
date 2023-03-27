#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "doctest.h"

struct Statistics {
  double mean{};
  double sigma{};
  double mean_err{};
  double median{};
};

class Sample {
  std::vector<double> entries_;

 public:
  int size() const { return entries_.size(); }

  void add(double x) { entries_.push_back(x); }

  auto& entries() { return entries_; }
  const auto& entries() const { return entries_; }

  bool remove(double x) {
    for (auto it = entries_.begin(), end = entries_.end(); it != end; ++it) {
      if (*it == x) {
        entries_.erase(it);
        return true;
      }
    }
    return false;
  }

  Statistics statistics() const {
    const int N{size()};

    if (N < 2) {
      throw std::runtime_error{"Not enough entries to run a statistics"};
    }

    double sum_x{};
    double sum_x2{};

    for (auto const& e : entries_) {
      sum_x += e;
      sum_x2 += e * e;
    }

    const double mean = sum_x / N;
    const double sigma = std::sqrt((sum_x2 - N * mean * mean) / (N - 1));
    const double mean_err = sigma / std::sqrt(N);

    auto entries{entries_};
    std::sort(entries.begin(), entries.end());

    const double median = N % 2 == 0
                              ? (entries[N / 2 - 1] + entries[N / 2]) * 0.5
                              : entries[N / 2];

    return {mean, sigma, mean_err, median};
  }

  Sample& operator+=(const Sample& r) {
    entries_.reserve(size() + r.size());
    entries_.insert(entries_.end(), r.entries().begin(), r.entries().end());
    return *this;
  }
};

Sample operator+(const Sample& l, const Sample& r) {
  Sample sum{l};
  return sum += r;
}

TEST_CASE("Testing the class handling a floating point data sample") {
  Sample sample;

  REQUIRE(sample.size() == 0);

  SUBCASE("Calling statistics() with no points throws") {
    CHECK_THROWS(sample.statistics());
  }

  SUBCASE("Calling statistics() with one point throws") {
    sample.add(4.0);
    CHECK_THROWS(sample.statistics());
  }

  SUBCASE("Calling statistics() with two points") {
    sample.add(1.0);
    sample.add(2.0);
    const auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(1.5));
    CHECK(result.sigma == doctest::Approx(0.70710678));
    CHECK(result.mean_err == doctest::Approx(0.5));
    CHECK(result.median == doctest::Approx(1.5));
  }

  SUBCASE("Calling statistics() with two identical points") {
    sample.add(10.0);
    sample.add(10.0);
    const auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(10.0));
    CHECK(result.sigma == doctest::Approx(0.0));
    CHECK(result.mean_err == doctest::Approx(0.0));
    CHECK(result.median == doctest::Approx(10.0));
  }

  SUBCASE("Calling statistics() with five points") {
    sample.add(1.5);
    sample.add(2.0);
    sample.add(2.5);
    sample.add(3.0);
    sample.add(3.5);
    const auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(2.5));
    CHECK(result.sigma == doctest::Approx(0.79056942));
    CHECK(result.mean_err == doctest::Approx(0.35355339));
    CHECK(result.median == doctest::Approx(2.5));
  }

  SUBCASE("Removing an existing point") {
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove(2.0) == true);
    CHECK(sample.size() == 1);
  }

  SUBCASE("Removing a non existing point") {
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove(1.9) == false);
    CHECK(sample.size() == 2);
  }

  SUBCASE("Testing sum operator") {
    sample.add(1);
    Sample sample_two;
    sample_two.add(2);
    auto sum = sample + sample_two;
    CHECK(sum.size() == 2);
    if (sum.size() == 2) {
      CHECK(sum.entries()[0] == doctest::Approx(1.0));
      CHECK(sum.entries()[1] == doctest::Approx(2.0));
    }
  }
}