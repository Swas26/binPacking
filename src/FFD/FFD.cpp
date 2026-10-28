#include "FFD.hpp"
#include <algorithm>
#include <list>

int binPackingFFD(std::vector<int> nums, int binCapacity) {
    sort(nums.begin(), nums.end());

    std::list<int> binSpaces = {binCapacity};

    bool needsBin;

    for (int num : nums) {
        needsBin = true;
        for (auto binIt = binSpaces.begin(); binIt != binSpaces.end(); ++binIt ) {
            if (num <= *binIt) {
                *binIt -= num;
                needsBin = false;
                break;
            }
        }
        if (needsBin) {
            binSpaces.push_back(binCapacity-num);
        }
    }

    return binSpaces.size();
}