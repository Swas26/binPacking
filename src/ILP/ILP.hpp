#pragma once
#include <vector>

int binPackingILP(std::vector<int> nums, int binCapacity);

int binPackingILP(std::vector<int> nums, int binCapacity, int maxBins, double timeLimit);

