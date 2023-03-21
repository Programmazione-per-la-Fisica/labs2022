#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

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

  // void add(double x) { entries_.push_back(x); }
  void add(double x);
  auto& entries() { return entries_; }
  const auto& entries() const { return entries_; }

  bool remove_entry(double x) {
    for (auto it = entries_.begin(), end = entries_.end(); it != end; ++it) {
      if ((*it) == x) {
        entries_.erase(it);
        return true;
      }
    }
    return false;
  }

  Statistics statistics() const {
    std::vector<double> entries{entries_};
    const int N = entries.size();

    if (N < 2) {
      throw std::runtime_error{"Not enough entries to run a statistics"};
    }

    double sum_x{};
    double sum_x2{};

    for (const auto& e : entries) {
      sum_x += e;
      sum_x2 += e * e;
    }

    const double mean = sum_x / N;
    const double sigma = std::sqrt((sum_x2 - N * mean * mean) / (N - 1));
    const double mean_err = sigma / std::sqrt(N);

    std::sort(entries.begin(), entries.end());

    const double median = N % 2 != 0
                              ? entries[N / 2]
                              : (entries[N / 2 - 1] + entries[N / 2]) * 0.5;

    return {mean, sigma, mean_err, median};
  }

  Sample& operator+=(const Sample& r) {
    entries_.insert(entries_.end(), r.entries().begin(), r.entries().end());
    return *this;
  }
};

inline Sample operator+(const Sample& l, const Sample& r) {
  Sample sum{l};
  return sum += r;
}

// auto statistics(Sample const & sample) {
//   // the following call would fail compilation if the fit method weren't
//   const return sample.statistics();
// }

#endif