#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

// A computationally intensive function for profiling
void compute_intensive_task() {
  const int size = 1000000;
  std::vector<int> data(size);

  // Initialize with random data
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 1000);

  for (int i = 0; i < size; ++i) {
    data[i] = dis(gen);
  }

  // Perform multiple sorting operations
  for (int iteration = 0; iteration < 10; ++iteration) {
    std::sort(data.begin(), data.end());
    std::reverse(data.begin(), data.end());
  }

  // Compute some statistics
  long long sum = 0;
  for (int val : data) {
    sum += val;
  }

  std::cout << "Sum: " << sum << std::endl;
  std::cout << "Average: " << static_cast<double>(sum) / size << std::endl;
}

// Memory allocation stress test
void memory_stress_test() {
  const int num_allocations = 1000;
  const int allocation_size = 1024 * 1024; // 1MB each

  std::vector<char *> allocations;

  for (int i = 0; i < num_allocations; ++i) {
    char *buffer = new char[allocation_size];

    // Write to the memory to ensure it's actually allocated
    for (int j = 0; j < allocation_size; j += 4096) {
      buffer[j] = static_cast<char>(i % 256);
    }

    allocations.push_back(buffer);

    // Small delay to make the profiling more interesting
    if (i % 100 == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  // Clean up
  for (char *buffer : allocations) {
    delete[] buffer;
  }

  std::cout << "Allocated and freed " << num_allocations << " MB of memory"
            << std::endl;
}

// CPU-intensive calculation
void fibonacci_calculation() {
  const int n = 40;

  auto fibonacci = [](int n) -> long long {
    if (n <= 1)
      return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
      long long temp = a + b;
      a = b;
      b = temp;
    }
    return b;
  };

  std::cout << "Calculating Fibonacci numbers..." << std::endl;
  for (int i = 1; i <= n; ++i) {
    long long result = fibonacci(i);
    if (i % 10 == 0) {
      std::cout << "F(" << i << ") = " << result << std::endl;
    }
  }
}

int main() {
  std::cout << "=== RealBench Test Application ===" << std::endl;
  std::cout << "Running performance-intensive tasks..." << std::endl;

  auto start = std::chrono::high_resolution_clock::now();

  // Run different computational tasks
  std::cout << "\n1. Running compute-intensive task..." << std::endl;
  compute_intensive_task();

  std::cout << "\n2. Running memory stress test..." << std::endl;
  memory_stress_test();

  std::cout << "\n3. Running Fibonacci calculation..." << std::endl;
  fibonacci_calculation();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "\n=== Performance Test Complete ===" << std::endl;
  std::cout << "Total execution time: " << duration.count() << " ms"
            << std::endl;

  return 0;
}
// Testing RealBench GitHub Action integration
// Final test trigger for RealBench GitHub Action
// changes after git backup restore an realbench
