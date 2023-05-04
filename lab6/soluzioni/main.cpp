#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>

#include "statistics.hpp"

int main() {
  try {
    pf::Sample data{};
    char cmd;

    int N{};
    double norm{};
    std::string filename;

    std::cout << "Valid commands: \n"
              << "- generate data [g N_ENTRIES]\n"
              << "- read a sample file [r FILE_NAME]\n"
              << "- compute statistics and print to screen [s]\n"
              << "- compute statistics and save it to file [o]\n"
              << "- print histogram to screen [h N_BINS NORM]\n"
              << "- quit [q]\n";

    std::default_random_engine eng;
    std::normal_distribution<double> dist;

    while (std::cin >> cmd) {
      if (cmd == 'g' && std::cin >> N) {
        std::generate_n(std::back_inserter(data), N,
                        [&]() { return dist(eng); });
        std::cout << "Data generated successfully" << '\n';
      }

      else if (cmd == 'r' && std::cin >> filename) {
        std::ifstream infile{filename};
        if (!infile) {
          throw std::runtime_error{"Impossible to open file!"};
        }
        double x;
        while (infile >> x) {
          data.add(x);
        }
        std::cout << "Input file read successfully" << '\n';
      }

      else if (cmd == 's') {
        std::cout << "- Generated data: " << data.size() << '\n';
        const auto stat = data.statistics();
        std::cout << "- mean: " << stat.mean << '\n'
                  << "- sigma: " << stat.sigma << '\n'
                  << "- mean error: " << stat.mean_err << '\n'
                  << "- median: " << stat.median << '\n';
      } else if (cmd == 'o') {
        std::ofstream outfile{"results.txt"};

        if (!outfile) {
          throw std::runtime_error{"Impossible to open file!"};
        }
        outfile << "- Read data:" << data.size() << '\n';
        const auto stat = data.statistics();
        outfile << "- mean: " << stat.mean << '\n'
                << "- sigma: " << stat.sigma << '\n'
                << "- mean error: " << stat.mean_err << '\n'
                << "- median: " << stat.median << '\n';

        std::cout << "Output file written successfully" << '\n';
      } else if (cmd == 'h' && std::cin >> N >> norm) {
        data.histogram(N, norm);
      }

      else if (cmd == 'q') {
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