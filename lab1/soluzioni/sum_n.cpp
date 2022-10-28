#include <iostream>

int main() {
  std::cout << "Provide the last (positive) number of the sum\n";
  int n;
  std::cin >> n;

  int sum{n * (n + 1) / 2};
  std::cout << "The sum of the first " << n
            << " positive natural numbers is: " << sum << '\n';
}