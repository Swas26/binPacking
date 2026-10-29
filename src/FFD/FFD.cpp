#include "FFD.hpp"
#include <algorithm>
#include <list>
#include <set>

// Primary author: Elan

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

int binPackingMFFD(std::vector<int> nums, int binCapacity) {

    int largeBound = binCapacity / 2;
    int mediumBound = binCapacity / 3;
    int smallBound = binCapacity / 6;

    std::multiset<int, std::greater<int>> largeNums;
    std::multiset<int, std::greater<int>> mediumNums;
    std::multiset<int, std::greater<int>> smallNums;
    std::multiset<int, std::greater<int>> tinyNums;

    std::list<int> binSpaces = {};

    for (int num : nums) {
        if (num > largeBound) {
            largeNums.insert(num);
        } else if (num > mediumBound) {
            mediumNums.insert(num);
        } else if (num > smallBound) {
            smallNums.insert(num);
        } else {
            tinyNums.insert(num);
        }
    }

    for (int largeNum : largeNums) {
        binSpaces.push_back(binCapacity-largeNum);
    }

    std::vector<bool> mediumFilled (binSpaces.size());

    int medIdx = 0;
    for (auto binIt = binSpaces.begin(); binIt != binSpaces.end() && !mediumNums.empty(); ++binIt) {
        if (*mediumNums.rbegin() < *binIt) {
            for (auto medIt = mediumNums.begin(); medIt != mediumNums.end(); ++medIt) {
                if (*medIt < *binIt) {
                    *binIt -= *medIt;
                    mediumNums.erase(medIt);
                    mediumFilled[medIdx] = true;
                    break;
                }
            }
        }
        medIdx++;
    }

    medIdx = 0;
    for (auto binIt = binSpaces.rbegin(); binIt != binSpaces.rend() && !smallNums.empty(); ++binIt) {
        if (!mediumFilled[medIdx]) {
            if (smallNums.size() > 1) {

                if (*std::next(smallNums.rbegin()) < *binIt) {
                    *binIt -= *std::prev(smallNums.end());
                    smallNums.erase(std::prev(smallNums.end()));

                    for (auto smallIt = smallNums.begin(); smallIt != smallNums.end(); ++smallIt) {
                        if (*smallIt < *binIt) {
                            *binIt -= *smallIt;
                            smallNums.erase(smallIt);
                            break;
                        }
                    }
                }
                
            }
        }
        medIdx++;
    }

    for (auto binIt = binSpaces.begin(); binIt != binSpaces.end(); ++binIt) {

        if (!mediumNums.empty() && *mediumNums.rbegin() < *binIt) {
            for (auto medIt = mediumNums.begin(); medIt != mediumNums.end(); ) {
                if (*medIt < *binIt) {
                    *binIt -= *medIt;
                    medIt = mediumNums.erase(medIt);
                } else {
                    medIt++;
                }
            }
        }

        if (!smallNums.empty() && *smallNums.rbegin() < *binIt) {
            for (auto smallIt = smallNums.begin(); smallIt != smallNums.end(); ) {
                if (*smallIt < *binIt) {
                    *binIt -= *smallIt;
                    smallIt = smallNums.erase(smallIt);
                } else {
                    smallIt++;
                }
            }
        }

        if (!tinyNums.empty() && *tinyNums.rbegin() < *binIt) {
            for (auto tinyIt = tinyNums.begin(); tinyIt != tinyNums.end(); ) {
                if (*tinyIt < *binIt) {
                    *binIt -= *tinyIt;
                    tinyIt = tinyNums.erase(tinyIt);
                } else {
                    tinyIt++;
                }
            }
        }

    }


    std::vector<int> remaining {};

    for (int num : mediumNums) {
        remaining.push_back(num);
    }
    for (int num : smallNums) {
        remaining.push_back(num);
    }
    for (int num : tinyNums) {
        remaining.push_back(num);
    }
    
    return binSpaces.size() + binPackingFFD(remaining, binCapacity);
}