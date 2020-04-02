import matplotlib.pyplot as plt
import os
import pandas as pd
import peakingduck as pkd
from peakingduck.core import NumericalData

import actigamma as ag


# change the filename here for a given spectrum
filename = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        '..', '..', 'reference', 'spectrum0.csv')

# read in from csv file
df = pd.read_csv(filename)

# get the counts
counts = NumericalData(df['count'].to_list())
energies = (NumericalData(df['lenergy'].to_list()) + NumericalData(df['uenergy'].to_list()))/2.0

# we don't trust anything less than 10 counts
class MyFilter(pkd.core.IProcess):      
    def go(self, data):
        newdata = [d if d > 10.0 else 0. for d in data]  
        return NumericalData(newdata)

# do some smoothing
# process manager
pm = pkd.core.PySimpleProcessManager(processes=[
    pkd.core.SavitzkyGolaySmoother(3),
    MyFilter()
])

filtereddata = pm.run(counts)

# find peaks 
pf = pkd.core.WindowPeakFinder(threshold=2.0, nouter=40, ninner=0, enforce_maximum=True)
peaks = pf.find(filtereddata)

# make a plot
fig, ax = plt.subplots()

for peak in peaks:
    peakvalue = counts[peak.index]#peak.value
    print(peakvalue, energies[peak.index])

    marker = plt.Line2D((energies[peak.index], energies[peak.index+1]), 
                                (peakvalue, peakvalue), lw=5., 
                                ls='-', marker='.', 
                                markersize=10, 
                                markerfacecolor='r', 
                                markeredgecolor='r', 
                                alpha=0.8)
    ax.add_line(marker)

plt.semilogy(energies, counts, 'k', label='raw')
plt.semilogy(energies, filtereddata, 'g', label='filtered')
plt.legend()
plt.show()
