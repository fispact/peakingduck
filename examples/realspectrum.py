import os
import pykingduck as pkd


# change the filename here for a given spectrum
filename = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        '..', 'reference', 'spectrum0.csv')

# read in from csv file
hist = pkd.core.SpectrumEnergyBased()
pkd.io.from_csv(hist, filename)
energies, counts = hist.X, hist.Y

# estimate and then remove background
ITERS = 15
background = hist.estimateBackground(ITERS)
hist.removeBackground(ITERS)
snippedCounts = hist.Y

# function to plot histogram
def getplotvalues(x,y):
    """
        Matplotlib hist is slow. Use a hist like plot with conventional line plot
    """
    assert (len(y) + 1) == len(x)
    X = []
    Y = []
    for i in range(len(x)-1):
        X.append(x[i])
        Y.append(y[i])
        X.append(x[i+1])
        Y.append(y[i])
    return X,Y

# plot the histogram
import matplotlib.pyplot as plt

plt.semilogy(*getplotvalues(energies, counts), 'k', linewidth=0.6, alpha=0.8, label="raw")
# plt.semilogy(*getplotvalues(energies, snippedCounts), 'r', linewidth=0.4, alpha=0.5, label="no background")
plt.semilogy(*getplotvalues(energies, background), 'g', linewidth=0.6, alpha=0.8, label="background")
plt.ylim(bottom=0.5, top=1e4)
plt.xlabel("Energy (eV)", fontsize=24)
plt.ylabel("Counts", fontsize=24)
plt.legend(fontsize=18)
plt.show()