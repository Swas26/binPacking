#pragma once
#include <vector>

enum class LowerBound {None, L1, L2};

int lowerBoundL1(const std::vector<int>& w, int C);
int lowerBoundL2(const std::vector<int>& w, int C);

int binPackingILP(const std::vector<int>& nums, int binCapacity);

int binPackingILP(const std::vector<int>& nums, int binCapacity, int maxBins, double timeLimit, LowerBound lb = LowerBound::L2);

