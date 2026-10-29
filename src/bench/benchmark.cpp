#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

#include "FFD.hpp"
#include "ILP.hpp"
#include "problems.hpp"

// Usage: ./binpack <timeLimitSeconds> <set> [set ...]
// Runs FFD and then the ILP (with FFD's bin count as maxBins) on every instance
// of the given BPPLIB sets, writing one row per instance to results/results.csv.

static void printUsage(const char* prog) {
    std::cerr << "usage: " << prog << " <timeLimitSeconds> <set> [set ...]\n"
              << "example: " << prog << " 10 Falkenauer_U\n";
}

static double msSince(std::chrono::steady_clock::time_point start) {
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    char* end;
    const double timeLimit = std::strtod(argv[1], &end);
    if (*end != '\0' || !(timeLimit > 0)) {
        std::cerr << "error: time limit must be a positive number of seconds, got \"" << argv[1] << "\"\n";
        printUsage(argv[0]);
        return 1;
    }
    const std::vector<std::string> sets(argv + 2, argv + argc);

    // Load everything before opening results.csv so a bad set name doesn't wipe old results
    std::vector<Problem> binPackingProblems;
    try {
        binPackingProblems = loadProblemSets("Data", sets);
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    const std::string resultsPath = "results/results.csv";
    std::error_code ec;
    std::filesystem::create_directories("results", ec);
    std::ofstream out(resultsPath);
    if (!out) {
        std::cerr << "error: cannot write " << resultsPath << "\n";
        return 1;
    }

    const size_t total = binPackingProblems.size();
    std::cout << "Running " << total << " instances, ILP time limit " << timeLimit << " s" << std::endl;

    out << std::fixed << std::setprecision(3);
    std::cout << std::fixed << std::setprecision(3);
    out << "instance,set,n,capacity,expected_bins,known_optimal,ffd_bins,ffd_ms,mffd_bins,mffd_ms,ilp_bins,ilp_ms,ilp_hit_time_limit" << std::endl;

    for (size_t i = 0; i < total; ++i) {
        const Problem& problem = binPackingProblems[i];

        auto start = std::chrono::steady_clock::now();
        const int ffdBins = binPackingFFD(problem.nums, problem.binCapacity);
        const double ffdMs = msSince(start);

        start = std::chrono::steady_clock::now();
        const int mffdBins = binPackingMFFD(problem.nums, problem.binCapacity);
        const double mffdMs = msSince(start);

        start = std::chrono::steady_clock::now();
        const int ilpBins = binPackingILP(problem.nums, problem.binCapacity, ffdBins, timeLimit);
        const double ilpMs = msSince(start);

        const bool hitTimeLimit = ilpMs >= 0.99 * timeLimit * 1000.0;

        // endl flushes, so finished rows survive a crash or Ctrl-C
        out << problem.name << ',' << problem.set << ',' << problem.nums.size() << ',' << problem.binCapacity << ','
            << problem.expectedNumBins << ',' << (problem.knownOptimal ? "true" : "false") << ','
            << ffdBins << ',' << ffdMs << ','
            << mffdBins << ',' << mffdMs << ','
            << ilpBins << ',' << ilpMs << ','
            << (hitTimeLimit ? "true" : "false") << std::endl;

        std::cout << "[" << (i + 1) << "/" << total << "] " << problem.name
                  << "  n=" << problem.nums.size() << "  expected=" << problem.expectedNumBins
                  << "  ffd=" << ffdBins << " (" << ffdMs << " ms)"
                  << "  mffd=" << mffdBins << " (" << mffdMs << " ms)"
                  << "  ilp=" << ilpBins << " (" << ilpMs << " ms" << (hitTimeLimit ? ", time limit" : "") << ")"
                  << std::endl;
    }

    std::cout << "wrote " << total << " rows to " << resultsPath << std::endl;
    return 0;
}
