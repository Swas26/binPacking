#include <vector>
#include <chrono>

int binPackingILP(std::vector<int> nums);

int binPackingFFD(std::vector<int> nums);

int main() {

    // Compare times and accuracy differences

    for (auto& problem : binPackingProblems) {
        // expectedNumBins = problem.expectedNumBins;
        auto start = std::chrono::high_resolution_clock::now();
        numBins = binPackingILP(nums);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        // Do something with duration
        // Do something with (numBins - expectedNumBins)
    }

}