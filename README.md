# RealBench Test C++ Project

A test C++ application designed to demonstrate RealBench profiling capabilities through GitHub Actions integration.

## Features

This application includes several computationally intensive tasks that generate interesting profiling data:

- **Sorting Algorithms**: Multiple sorting and reverse operations on large datasets
- **Memory Stress Test**: Dynamic memory allocation and deallocation patterns
- **Fibonacci Calculations**: CPU-intensive mathematical computations

## Building

### Prerequisites
- CMake 3.12 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Build the project
cmake --build . --parallel

# Run the application
./bin/realbench-test
```

## RealBench Integration

This project is configured to work with RealBench GitHub Actions for automatic profiling on every push and pull request.

The profiling will capture:
- CPU usage patterns during sorting operations
- Memory allocation patterns
- Computational bottlenecks in Fibonacci calculations
- Overall performance characteristics

## Expected Output

When running the application, you should see output similar to:

```
=== RealBench Test Application ===
Running performance-intensive tasks...

1. Running compute-intensive task...
Sum: 500500000
Average: 500.5

2. Running memory stress test...
Allocated and freed 1000 MB of memory

3. Running Fibonacci calculation...
Calculating Fibonacci numbers...
F(10) = 55
F(20) = 6765
F(30) = 832040
F(40) = 102334155

=== Performance Test Complete ===
Total execution time: 1234 ms
```

## GitHub Actions

The `.github/workflows/realbench.yml` file automatically profiles this application using RealBench on:
- Every push to main/master branches
- Every pull request

Results are posted as PR comments with performance insights and optimization suggestions.
