import os
import pandas as pd
import peakingduck as pkd

# change the filename here for a given spectrum
filename = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        '..', 'reference', 'spectrum0.csv')

# read in from csv file
df = pd.read_csv(filename)

counts = pkd.core.NumericalData(df['count'].to_list())
snipped = counts.snip(range(1, 20))
snipped2 = counts.snip(range(19, 0, -1))

import matplotlib.pyplot as plt

plt.semilogy(range(len(counts)), counts, 'k', label='raw')
plt.semilogy(range(len(counts)), snipped, 'r', label='snip')
plt.semilogy(range(len(counts)), snipped2, 'g', label='snip2')

plt.legend()
plt.show()


