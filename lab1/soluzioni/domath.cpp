#include <iostream>

int main() {
  std::cout << "Provide the first number\n";
  int i;
  std::cin >> i;
  std::cout << "Provide the second number\n";
  int j;
  std::cin >> j;

  int sum{i + j};
  std::cout << "The sum is: " << sum << '\n';

  if (j == 0) {
    std::cout << "The divisor cannot be zero\n";
  } else if (i % j != 0) {
    std::cout << i << " is NOT a multiple of " << j << '\n';
  } else {
    std::cout << i << " is a multiple of " << j << '\n';
  }
}