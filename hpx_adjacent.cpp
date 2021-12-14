#include <hpx/hpx_main.hpp>
#include <hpx/modules/algorithms.hpp>
#include <hpx/hpx.hpp>

#include <chrono>
#include <random>
#include <vector>

int test_count = 100;

unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);

void measure_hpx_adjacent_difference(std::vector<int> const &vec) {

hpx::adjacent_difference(hpx::execution::par, vec.begin(), vec.end());
}

double averageout_hpx_adjacent_difference(std::vector<int> const &vec) {

  auto start = std::chrono::high_resolution_clock::now();

  // average out 100 executions to avoid varying results
  for (auto i = 0; i < test_count; i++) {
    measure_hpx_adjacent_difference(vec);
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> elapsed_seconds = end - start;
  return elapsed_seconds.count() / test_count;
}

int main(int argc, char *argv[]) {
  int n;
  if (argv[1] == NULL) {
    n = 1000000;
  } else {
    n = std::atoi(argv[1]);
  }

  std::vector<int> vec(n);
  std::fill(std::begin(vec), std::end(vec), gen() % 1000);

  auto time = averageout_hpx_adjacent_difference(vec);

  std::cout << "[HPX]: " << n << ", " << hpx::get_num_worker_threads() << ", "
            << time << std::endl;

  return 0;
}