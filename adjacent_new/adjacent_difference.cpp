#include <hpx/local/init.hpp>
#include <hpx/modules/algorithms.hpp>
#include <hpx/modules/program_options.hpp>
#include <hpx/modules/testing.hpp>
#include <hpx/parallel/algorithms/adjacent_difference.hpp>

#include <array>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <vector>

void measureAdjacent_differenceAlgorithms() {

  std::size_t start = 32;
  std::size_t till = 1 << 20;

  const auto NUM_ITERATIONS = 5;

  std::vector<std::array<double, 3>> data;
  for (size_t s = start; s <= till; s *= 2) {
    std::vector<int> arr(s);
    std::iota(std::begin(arr), std::end(arr), 1);

    double seqTime = 0;
    double parTime = 0;

    for (int i = 0; i <= NUM_ITERATIONS; i++) {
      std::vector<int> res(s);
      auto t1 = std::chrono::high_resolution_clock::now();
      hpx::adjacent_difference(hpx::execution::seq, arr.begin(), arr.end(),
                               res.begin());

      auto end1 = std::chrono::high_resolution_clock::now();

      if (NUM_ITERATIONS < 5) {
        continue;
      }
      std::chrono::duration<double> time_span1 =
          std::chrono::duration_cast<std::chrono::duration<double>>(end1 - t1);
      seqTime += time_span1.count();
    }
    for (int i = 0; i < NUM_ITERATIONS + 5; i++) {
      std::vector<int> res1(s);
      auto t2 = std::chrono::high_resolution_clock::now();
      hpx::adjacent_difference(hpx::execution::par, arr.begin(), arr.end(),
                               res1.begin());

      auto end2 = std::chrono::high_resolution_clock::now();

      if (NUM_ITERATIONS < 5) {
        continue;
      }

      std::chrono::duration<double> time_span2 =
          std::chrono::duration_cast<std::chrono::duration<double>>(end2 - t2);

      parTime += time_span2.count();
    }

    seqTime /= NUM_ITERATIONS;
    parTime /= NUM_ITERATIONS;

    data.push_back(std::array<double, 3>{(double)s, seqTime, parTime});
    std::cout << "n : " << s << '\n';
    std::cout << "seq: " << seqTime << '\n';
    std::cout << "par: " << parTime << "\n\n";
  }
  //   std::ofstream outputFile(filenames[(ALGORITHM)alg]);
  for (auto &d : data) {
    std::cout << d[0] << "," << d[1] << "," << d[2] << ","
              << ",\n";
  }
  //   }
}
int hpx_main(hpx::program_options::variables_map &) {
  measureAdjacent_differenceAlgorithms();

  return hpx::local::finalize();
}
int main(int argc, char *argv[]) {
  std::vector<std::string> cfg;
  cfg.push_back("hpx.os_threads=all");
  hpx::local::init_params init_args;
  init_args.cfg = cfg;

  // Initialize and run HPX.
  HPX_TEST_EQ_MSG(hpx::local::init(hpx_main, argc, argv, init_args), 0,
                  "HPX main exited with non-zero status");

  return hpx::util::report_errors();
}