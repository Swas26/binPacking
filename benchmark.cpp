#include <vector>
#include <chrono>

#include "FFD.hpp"

int binPackingILP(std::vector<int> nums, int binCapacity);

int main() {

    // Compare times and accuracy differences

    for (auto& problem : binPackingProblems) {
        // expectedNumBins = problem.expectedNumBins;
        auto start = std::chrono::high_resolution_clock::now();
        // numBins = binPackingFFD(problem.nums, problem.binCapacity);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // Do something with duration
        // Do something with (numBins - expectedNumBins)
    }

    // Repeat for ILP

}