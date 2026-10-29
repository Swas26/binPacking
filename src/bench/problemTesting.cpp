#include <iostream>
#include <vector>

#include "FFD.hpp"
#include "ILP.hpp"

// Small sanity suite: the ILP must find the optimum and FFD must never beat it.
// Returns non-zero if any case fails.

struct TestCase {
    std::vector<int> nums;
    int binCapacity;
    int optimal;
};

int main() {
    const std::vector<TestCase> cases = {
        {{5, 4, 4, 3, 2, 2}, 10, 2},
        {{11, 11, 11, 11, 1, 1, 1}, 20, 4},
        {{2, 2, 2, 3, 3, 3, 7, 7, 7}, 10, 4},
        {{10, 10, 10}, 10, 3},
        {{1}, 10, 1},
    };

    int failures = 0;
    for (const TestCase& t : cases) {
        // Same call pattern as the benchmark: FFD's count sets the ILP's bin slots
        const int ffdBins = binPackingFFD(t.nums, t.binCapacity);
        const int mffdBins = binPackingMFFD(t.nums, t.binCapacity);
        const int ilpBins = binPackingILP(t.nums, t.binCapacity, ffdBins, 10.0);
        const bool ok = ilpBins == t.optimal && 9 * ffdBins <= t.optimal*11 + 6 && 60 * mffdBins <= t.optimal*71 + 60;
        if (!ok) ++failures;

        std::cout << (ok ? "PASS" : "FAIL") << "  {";
        for (size_t i = 0; i < t.nums.size(); ++i) std::cout << (i > 0 ? "," : "") << t.nums[i];
        std::cout << "}/" << t.binCapacity << "  optimal=" << t.optimal
                  << "  ilp=" << ilpBins << "  ffd=" << ffdBins << "  mffd=" << mffdBins << "\n";
    }

    if (failures > 0) {
        std::cout << failures << " of " << cases.size() << " tests failed\n";
        return 1;
    }
    std::cout << "all " << cases.size() << " tests passed\n";
    return 0;
}
