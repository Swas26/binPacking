CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall -Isrc/FFD -Isrc/ILP -Isrc/bench

# BREW_DIRECTORY = /opt/homebrew
BREW_DIRECTORY ?= $(shell brew --prefix 2>/dev/null || echo /usr/local)

# HiGHS
HIGHS_INC = -isystem $(BREW_DIRECTORY)/include/highs
HIGHS_LIB = -L$(BREW_DIRECTORY)/lib -lhighs -Wl,-rpath,$(BREW_DIRECTORY)/lib

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

# make run: every set in Data/bpplib, one ./binpack process per set, JOBS at a time,
# each writing results/<set>.csv. Change TIME_LIMIT here, or for one run: make run TIME_LIMIT=60
# also: make run SETS="Falkenauer_T Hard28"   (only those sets)
TIME_LIMIT ?= 10
JOBS ?= $(shell sysctl -n hw.perflevel0.physicalcpu 2>/dev/null || echo 4)
SETS ?= $(notdir $(patsubst %/,%,$(wildcard Data/bpplib/*/)))

run: binpack
	printf '%s\n' $(SETS) | xargs -P $(JOBS) -I{} ./binpack $(TIME_LIMIT) {}

clean:
	rm -f binpack test

.PHONY: run clean
