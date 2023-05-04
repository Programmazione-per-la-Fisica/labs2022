#include "statistics.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>

namespace pf {

void Sample::add(double x) { entries_.push_back(x); }
/*
void Sample::add(double x) {
  auto it = std::upper_bound(entries_.begin(), entries_.end(), x);
  entries_.insert(it, x);
}
*/
bool Sample::remove(double x) {
  auto const it = std::find(entries_.begin(), entries_.end(), x);

  if (it != entries_.end()) {
    entries_.erase(it);
    return true;
  }
  return false;
}

Statistics Sample::statistics() const {
  const int N{size()};

  if (N < 2) {
    throw std::runtime_error{"Not enough entries to run a statistics"};
  }

  struct Sums {
    double x;
    double x2;
  };

  const auto sums = std::accumulate(entries_.begin(), entries_.end(), Sums{},
                                    [](Sums s, double x) {
                                      s.x += x;
                                      s.x2 += x * x;
                                      return s;
                                    });

  const double mean = sums.x / N;
  const double sigma = std::sqrt((sums.x2 - N * mean * mean) / (N - 1));
  const double mean_err = sigma / std::sqrt(N);

  auto entries{entries_};
  std::sort(entries.begin(), entries.end());

  const double median =
      N % 2 == 0 ? (entries[N / 2 - 1] + entries[N / 2]) * 0.5 : entries[N / 2];

  return {mean, sigma, mean_err, median};
}

Sample& Sample::operator+=(const Sample& r) {
  entries_.reserve(size() + r.size());
  entries_.insert(entries_.end(), r.entries().begin(), r.entries().end());
  return *this;
}

Sample operator+(const Sample& l, const Sample& r) {
  Sample sum{l};
  return sum += r;
}

void Sample::histogram(int n_bins, double norm) {

  if (size() < 1) {
    throw std::runtime_error{"Not enough entries to draw a histogram"};
  }

  if (n_bins <= 0) {
    throw std::runtime_error{"Parameter n_bins must be positive"};
  }

  if (norm <= 0) {
    throw std::runtime_error{"Parameter norm must be positive"};
  }

  auto entries{entries_};
  std::sort(entries.begin(), entries.end());

  const double min{entries.front()};
  const double max{entries.back()};
  const double delta{(max - min) / n_bins};

  auto l_edge = entries.begin();

  for (int i = 0; i < n_bins; ++i) {
    auto r_edge = upper_bound(l_edge, entries.end(), min + delta * (i + 1));
    auto count = r_edge - l_edge;
    l_edge = r_edge;

    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(2)
              << std::showpos << min + delta * (i + 0.5) << " |";

    std::string row(std::round(count / norm), '-');
    std::cout << row << "|\n";
  }
}
}  // namespace pf
