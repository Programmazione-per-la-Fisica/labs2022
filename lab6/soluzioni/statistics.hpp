#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

namespace pf {
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

  auto& entries() { return entries_; }
  const auto& entries() const { return entries_; }

  void add(double);
  bool remove(double);

  using value_type = double;
  void push_back(double x) { add(x); };

  Statistics statistics() const;

  Sample& operator+=(const Sample&);

  void histogram(int, double);
};

Sample operator+(const Sample&, const Sample&);

}  // namespace pf

#endif
