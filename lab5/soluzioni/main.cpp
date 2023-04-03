#include <iostream>
#include <limits>

#include "statistics.hpp"

int main() {
  try {
    pf::Sample data{};

    std::cout << "Valid commands: \n"
              << "- add a value [a val]\n"
              << "- remove a value [r val]\n"
              << "- compute statistics [s]\n"
              << "- quit [q]\n";

    char cmd{};

    while (std::cin >> cmd) {
      double val{};
      
      if (cmd == 'a' && std::cin >> val) {
        data.add(val);
      } else if (cmd == 'r' && std::cin >> val) {
        if (!data.remove(val)) {
          std::cout << val << " not found in the data sample, not removed\n";
        }
      } else if (cmd == 's') {
        std::cout << "Data sample size: " << data.size() << '\n';
        const auto stat = data.statistics();
        std::cout << "- mean: " << stat.mean << '\n'
                  << "- sigma: " << stat.sigma << '\n'
                  << "- mean error: " << stat.mean_err << '\n'
                  << "- median: " << stat.median << '\n';
      } else if (cmd == 'q') {
        return EXIT_SUCCESS;
      } else {
        std::cout << "Bad format, insert a new command\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }

  } catch (std::exception const& e) {
    std::cerr << "Caught exception: '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}
