#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
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

// Cache-unfriendly linked list traversal (high cache miss rate)
struct ListNode {
  int value;
  ListNode* next;
  ListNode(int v) : value(v), next(nullptr) {}
};

void cache_unfriendly_traversal() {
  std::cout << "Running cache-unfriendly linked list traversal..." << std::endl;
  const int num_nodes = 500000;

  // Allocate nodes scattered in memory
  std::vector<ListNode*> nodes;
  nodes.reserve(num_nodes);
  for (int i = 0; i < num_nodes; ++i) {
    nodes.push_back(new ListNode(i));
  }

  // Shuffle to break cache locality
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(nodes.begin(), nodes.end(), gen);

  // Link them in shuffled order
  for (size_t i = 0; i < nodes.size() - 1; ++i) {
    nodes[i]->next = nodes[i + 1];
  }
  nodes.back()->next = nullptr;

  // Traverse (cache misses galore)
  ListNode* head = nodes[0];
  volatile long long sum = 0;
  int iterations = 50;
  for (int iter = 0; iter < iterations; ++iter) {
    ListNode* current = head;
    while (current) {
      sum += current->value;
      current = current->next;
    }
  }

  // Cleanup
  for (auto* node : nodes) {
    delete node;
  }
  std::cout << "Cache-unfriendly sum (should be ~" << num_nodes * (num_nodes - 1) / 2 * iterations << "): " << sum << std::endl;
}

// Branch-heavy unpredictable workload
void branch_misprediction_test() {
  std::cout << "Running branch misprediction test..." << std::endl;
  const int size = 10000000;
  std::vector<int> data(size);

  // Random data - unpredictable branches
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 99);
  for (int i = 0; i < size; ++i) {
    data[i] = dis(gen);
  }

  volatile long long sum = 0;
  // Unpredictable branches
  for (int val : data) {
    if (val < 30) {
      sum += val * 2;
    } else if (val < 60) {
      sum += val * 3;
    } else if (val < 80) {
      sum += val * 5;
    } else {
      sum += val * 7;
    }
  }
  std::cout << "Branch test sum: " << sum << std::endl;
}

// Deep recursion stack
int deep_recursive_call(int depth, int iterations) {
  if (depth <= 0) {
    volatile long long sum = 0;
    for (int i = 0; i < iterations; ++i) {
      sum += i;
    }
    return static_cast<int>(sum % 1000000);
  }
  return deep_recursive_call(depth - 1, iterations) + depth;
}

void deep_recursion_test() {
  std::cout << "Running deep recursion test..." << std::endl;
  volatile int result = 0;
  for (int i = 0; i < 100; ++i) {
    result += deep_recursive_call(200, 1000);
  }
  std::cout << "Deep recursion result: " << result << std::endl;
}

// Virtual function dispatch overhead
class BaseProcessor {
public:
  virtual ~BaseProcessor() = default;
  virtual int process(int x) = 0;
};

class ProcessorA : public BaseProcessor {
public:
  int process(int x) override {
    return x * 2 + 1;
  }
};

class ProcessorB : public BaseProcessor {
public:
  int process(int x) override {
    return x * 3 - 1;
  }
};

class ProcessorC : public BaseProcessor {
public:
  int process(int x) override {
    return x * x + x;
  }
};

void virtual_dispatch_test() {
  std::cout << "Running virtual dispatch test..." << std::endl;
  const int num_objects = 1000;
  const int iterations = 10000;

  std::vector<std::unique_ptr<BaseProcessor>> processors;
  processors.reserve(num_objects);

  // Mix of different processors
  for (int i = 0; i < num_objects; ++i) {
    switch (i % 3) {
      case 0: processors.push_back(std::make_unique<ProcessorA>()); break;
      case 1: processors.push_back(std::make_unique<ProcessorB>()); break;
      case 2: processors.push_back(std::make_unique<ProcessorC>()); break;
    }
  }

  volatile long long total = 0;
  for (int iter = 0; iter < iterations; ++iter) {
    for (int i = 0; i < num_objects; ++i) {
      total += processors[i]->process(i + iter);
    }
  }
  std::cout << "Virtual dispatch total: " << total << std::endl;
}

// STL-heavy template operations
void stl_heavy_template_test() {
  std::cout << "Running STL heavy template test..." << std::endl;
  const int num_elements = 100000;

  // Complex template types
  std::map<std::string, std::vector<std::pair<int, double>>> complex_map;
  std::unordered_map<int, std::set<std::string>> reverse_lookup;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 1000);
  std::uniform_real_distribution<> ddis(0.0, 1.0);

  // Populate complex structures
  for (int i = 0; i < num_elements; ++i) {
    std::string key = "key_" + std::to_string(i);
    std::vector<std::pair<int, double>> vec;
    for (int j = 0; j < 10; ++j) {
      vec.push_back({dis(gen), ddis(gen)});
    }
    complex_map[key] = vec;
    reverse_lookup[i % 100].insert(key);
  }

  // Heavy lookup operations
  volatile double sum = 0.0;
  for (int iter = 0; iter < 100; ++iter) {
    for (const auto& [key, vec] : complex_map) {
      for (const auto& [i, d] : vec) {
        sum += i * d;
      }
    }
  }

  // Set operations
  std::set<std::string> intersection;
  for (int i = 0; i < 50; ++i) {
    auto& set1 = reverse_lookup[i];
    auto& set2 = reverse_lookup[i + 50];
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::inserter(intersection, intersection.begin()));
  }

  std::cout << "STL heavy test sum: " << sum << ", intersections: " << intersection.size() << std::endl;
}

// Lock contention test
void lock_contention_test() {
  std::cout << "Running lock contention test..." << std::endl;
  const int num_threads = 8;
  const int iterations = 100000;

  std::mutex mtx;
  std::shared_mutex shared_mtx;
  std::atomic<long long> atomic_counter{0};
  volatile long long protected_counter = 0;

  std::vector<std::thread> threads;

  // Worker function with mixed locking
  auto worker = [&](int thread_id) {
    std::random_device rd;
    std::mt19937 gen(rd() + thread_id);
    std::uniform_int_distribution<> dis(0, 9);

    for (int i = 0; i < iterations; ++i) {
      int op = dis(gen);
      if (op < 4) {
        // Exclusive lock
        std::lock_guard<std::mutex> lock(mtx);
        protected_counter += thread_id;
      } else if (op < 7) {
        // Shared lock
        std::shared_lock<std::shared_mutex> lock(shared_mtx);
        volatile long long read_val = protected_counter;
        (void)read_val;
      } else {
        // Atomic operation
        atomic_counter.fetch_add(1, std::memory_order_relaxed);
      }
    }
  };

  // Launch threads
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(worker, i);
  }
  for (auto& t : threads) {
    t.join();
  }
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Lock contention test complete: " << duration.count() << "ms, atomic: " << atomic_counter.load()
            << ", protected: " << protected_counter << std::endl;
}

// Memory fragmentation simulation
void memory_fragmentation_test() {
  std::cout << "Running memory fragmentation test..." << std::endl;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> size_dis(64, 65536);

  std::vector<std::unique_ptr<char[]>> allocations;
  std::vector<size_t> sizes;

  // Random size allocations
  for (int i = 0; i < 5000; ++i) {
    size_t size = size_dis(gen);
    allocations.push_back(std::make_unique<char[]>(size));
    sizes.push_back(size);

    // Touch memory
    for (size_t j = 0; j < size; j += 4096) {
      allocations.back()[j] = static_cast<char>(i % 256);
    }

    // Free every third allocation to fragment
    if (i % 3 == 0 && !allocations.empty()) {
      allocations.pop_back();
      sizes.pop_back();
    }
  }

  // Reallocate with different sizes to stress allocator
  for (int phase = 0; phase < 5; ++phase) {
    for (size_t i = 0; i < allocations.size(); ++i) {
      allocations[i].reset();
      size_t new_size = size_dis(gen);
      allocations[i] = std::make_unique<char[]>(new_size);
      for (size_t j = 0; j < new_size; j += 4096) {
        allocations[i][j] = static_cast<char>(phase);
      }
    }
  }

  std::cout << "Memory fragmentation test complete, " << allocations.size() << " blocks" << std::endl;
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

  std::cout << "\n4. Running cache-unfriendly traversal..." << std::endl;
  cache_unfriendly_traversal();

  std::cout << "\n5. Running branch misprediction test..." << std::endl;
  branch_misprediction_test();

  std::cout << "\n6. Running deep recursion test..." << std::endl;
  deep_recursion_test();

  std::cout << "\n7. Running virtual dispatch test..." << std::endl;
  virtual_dispatch_test();

  std::cout << "\n8. Running STL heavy template test..." << std::endl;
  stl_heavy_template_test();

  std::cout << "\n9. Running lock contention test..." << std::endl;
  lock_contention_test();

  std::cout << "\n10. Running memory fragmentation test..." << std::endl;
  memory_fragmentation_test();

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
// updating sampler.cpp for better addr2line
