#pragma once
#include <string>
#include <vector>

// One BPPLIB instance joined with its row in Data/bpplib_solutions.csv
struct Problem {
    std::string name;           // "SetName/file", same key as the solutions CSV
    std::string set;
    std::vector<int> nums;
    int binCapacity = 0;
    int expectedNumBins = -1;   // best known upper bound (best_ub), -1 if not in the CSV
    bool knownOptimal = false;  // true if the CSV marks the instance "solved"
};

// Loads every instance in <dataDir>/bpplib/<set>/ for each set, in sorted path order.
// Throws std::runtime_error for a missing set or a malformed file.
std::vector<Problem> loadProblemSets(const std::string& dataDir, const std::vector<std::string>& sets);
