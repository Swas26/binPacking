#include "ILP.hpp"
#include "Highs.h"

/* Exact bin packing ILP via HiGHS */
/* Primary auth: Swas*/


int binPackingILP(std::vector<int> nums, int binCapacity){
    return binPackingILP(nums, binCapacity, 0, 60.0);
}

int binPackingILP(std::vector<int> nums, int binCapacity, int maxBins, double timeLimit){

    const int n  = static_cast<int>(nums.size());
    if (n == 0) return 0;

    const int U = (maxBins > 0 && maxBins < n) ? maxBins : n;
    const double C = binCapacity;

    auto yIdx = [&](int j) { return j; };
    auto xIdx = [&](int i, int j) { return U + i * U + j; };

    const int numVars = U + n * U;

    Highs h;
    h.setOptionValue("output_flag", false);
    h.setOptionValue("threads", 1);
    h.setOptionValue("time_limit", timeLimit);


    std::vector<double> lower(numVars, 0.0);
    std::vector<double> upper(numVars, 1.0);

    for (int i = 0; i < n; ++i){

        for (int j = i + 1; j < U; ++j){
            upper[xIdx(i, j)] = 0.0;
        }
    }

    h.addVars(numVars, lower.data(), upper.data());

    std::vector<HighsVarType> integrality(numVars, HighsVarType::kInteger);
    h.changeColsIntegrality(0, numVars - 1, integrality.data());

    for (int j = 0; j < U; ++j){
        h.changeColCost(yIdx(j), 1.0);
    }

    std::vector<HighsInt> idx;
    std::vector<double> val;


    for ( int i = 0; i < n; ++i){

        idx.clear(); val.clear(); 
        for (int k = 0; k < U; ++k){

            idx.push_back( xIdx(i, k));
            val.push_back( 1.0);
        }

        h.addRow(1.0, 1.0, idx.size(), idx.data(), val.data());
    }

    for ( int j = 0; j < U; ++j){

        idx.clear();  val.clear();
        for ( int i = 0; i < n; ++i){
            idx.push_back( xIdx(i, j));
            val.push_back(nums[i]);
        }
        idx.push_back(yIdx(j));  val.push_back(-C);

        h.addRow(-kHighsInf, 0.0, idx.size(), idx.data(), val.data());
    }


    for ( int j = 0; j + 1 < U; ++j){

        HighsInt rIdx[2] = { yIdx(j), yIdx(j +1) };
        double rVal[2] = {1.0, -1.0};
        h.addRow(0.0, kHighsInf, 2, rIdx, rVal);
    }

    h.run();

    if (h.getInfo().primal_solution_status != kSolutionStatusFeasible) return -1;

    const std::vector<double>& v = h.getSolution().col_value;
    int bins = 0;
    for ( int j = 0; j < U; ++j){
        if ( v[yIdx(j)] > 0.5) ++bins;
    }
    return bins;


}