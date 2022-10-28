#include <iostream>

int main() {
  std::cout << "Provide the lowest (positive) extreme of the interval\n";
  int a;
  std::cin >> a;
  std::cout << "Provide the highest (positive) extreme of the interval\n";
  int b;
  std::cin >> b;

  int sum{(a + b) * (b - a + 1) / 2};
  std::cout << "The sum of the natural numbers between " << a << " and " << b
            << " is: " << sum << '\n';
}