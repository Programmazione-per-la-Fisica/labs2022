#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "statistics.hpp"
#include "statistics.hpp"

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
    auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(1.5));
    CHECK(result.sigma == doctest::Approx(0.70710678));
    CHECK(result.mean_err == doctest::Approx(0.5));
    CHECK(result.median == doctest::Approx(1.5));
  }

  SUBCASE("Calling statistics() with two identical points") {
    sample.add(10.0);
    sample.add(10.0);
    auto result = sample.statistics();
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
    auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(2.5));
    CHECK(result.sigma == doctest::Approx(0.79056942));
    CHECK(result.mean_err == doctest::Approx(0.35355339));
    CHECK(result.median == doctest::Approx(2.5));
  }

  SUBCASE("Removing an existing point") {
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove_entry(2.0) == true);
    CHECK(sample.size() == 1);
  }

  SUBCASE("Removing a non existing point") {
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove_entry(1.9) == false);
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
