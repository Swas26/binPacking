CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall -Isrc/FFD -Isrc/ILP -Isrc/bench

# HiGHS
HIGHS_INC = -isystem /opt/homebrew/include/highs
HIGHS_LIB = -L/opt/homebrew/lib -lhighs -Wl,-rpath,/opt/homebrew/lib

ALGOS = src/FFD/FFD.cpp src/ILP/ILP.cpp
HEADERS = src/FFD/FFD.hpp src/ILP/ILP.hpp src/bench/problems.hpp
BENCH_SRCS = $(ALGOS) src/bench/problems.cpp src/bench/benchmark.cpp
TEST_SRCS = $(ALGOS) src/bench/problemTesting.cpp

# Main benchmark program
binpack: $(BENCH_SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(HIGHS_INC) $(BENCH_SRCS) -o binpack $(HIGHS_LIB)

# Test suite (exits non-zero on failure)
test: $(TEST_SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(HIGHS_INC) $(TEST_SRCS) -o test $(HIGHS_LIB)

run: binpack
	./binpack 10 Falkenauer_U

clean:
	rm -f binpack test

.PHONY: run clean
