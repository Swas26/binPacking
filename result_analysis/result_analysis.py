import os
import pandas as pd
import matplotlib.pyplot as plt

# Primary auth: Elan

resultFiles = os.listdir("results")

print(resultFiles)

resultSets = []

for resultFile in resultFiles:
    result = pd.read_csv(os.path.join("results",resultFile))
    resultSets.append(result)

# Every graph has 3 lines for each alg + optimal

# Output Plots
for resultSet in resultSets:
    plt.plot(resultSet['ffd_bins'], linewidth = 3)
    plt.plot(resultSet['mffd_bins'])
    plt.plot(resultSet['ilp_bins'], linestyle='dashdot')
    plt.plot(resultSet['expected_bins'], linestyle='--')

    plt.xlabel('Problem Instance')
    plt.ylabel('Number of Bins')
    plt.title(f"Algorithm Output Comparison - {resultSet['set'][0]} Dataset")

    plt.ylim((0,1.05*max(resultSet['ffd_bins'].max(), resultSet['mffd_bins'].max(), resultSet['ilp_bins'].max())))

    plt.legend(["FFD", "MFFD", "ILP", "Optimal"])
    plt.savefig(f"result_analysis/graphs/bin_comparison_{resultSet['set'][0]}.png", dpi=300)
    plt.close()

# Error Plots [for each set, per instance, sorted by n / average across each set] {Additive / Multiplicative}

# Add limit line for multi

# Time Plots [for each set, per instance, sorted by n / average across each set]