import os
import pandas as pd
import numpy as np
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
# for resultSet in resultSets:
#     plt.plot(resultSet['ffd_bins'], linewidth = 3)
#     plt.plot(resultSet['mffd_bins'])
#     plt.plot(resultSet['ilp_bins'], linestyle='dashdot')
#     plt.plot(resultSet['expected_bins'], linestyle='--')

#     plt.xlabel('Problem Instance')
#     plt.ylabel('Number of Bins')
#     plt.title(f"Algorithm Output Comparison - {resultSet['set'][0]} Dataset")

#     plt.ylim((0,1.05*max(resultSet['ffd_bins'].max(), resultSet['mffd_bins'].max(), resultSet['ilp_bins'].max())))

#     plt.legend(["FFD", "MFFD", "ILP", "Optimal"])
#     plt.savefig(f"result_analysis/graphs/bin_comparison_{resultSet['set'][0]}.png", dpi=300)
#     plt.close()

# Error Plots [for each set, per instance, sorted by n / average across each set] {Additive / Multiplicative}

algs = ["FFD","MFFD","ILP"]
datasetNames = [resultSet['set'][0] for resultSet in resultSets]

additive_error = [
    [(resultSet['ffd_bins']-resultSet['expected_bins']).mean() for resultSet in resultSets],
    [(resultSet['mffd_bins']-resultSet['expected_bins']).mean() for resultSet in resultSets],
    [(resultSet.loc[resultSet['ilp_bins']!=-1,'ilp_bins']-resultSet.loc[resultSet['ilp_bins']!=-1,'expected_bins']).mean() for resultSet in resultSets],
]

plt.title(f"Average Additive Error Comparison")

plt.xlabel('Problem Set')
plt.ylabel('Average Number of Bins above Optimal')

x = np.arange(len(datasetNames))
width = 0.2
offset = 0
for alg_error in additive_error:
    plt.bar(x + offset, alg_error, width)
    offset += width
plt.xticks(x, datasetNames, rotation=90)
plt.legend(algs)
plt.tight_layout()

plt.savefig(f"result_analysis/graphs/additive_error.png", dpi=300)
plt.close()

# plt.plot(*11/9)
# plt.plot(*71/60)

# Add limit line for multi

# Time Plots [for each set, per instance, sorted by n / average across each set]