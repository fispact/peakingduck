import os
import peakingduck as pkd


# change the filename here for a given spectrum
filename = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        '..', 'reference', 'spectrum0.csv')

# read in from csv file
hist_raw = pkd.core.SpectrumEnergyBased()
pkd.io.from_csv(hist_raw, filename)
energies, counts = hist_raw.X, hist_raw.Y

# estimate and then remove background
ITERS = range(1, 21)
# forward windowing
background_forw = hist_raw.estimateBackground(ITERS)
# backward windowing
background_back = hist_raw.estimateBackground(list(reversed(ITERS)))
hist_raw.removeBackground(ITERS)
snippedCounts = hist_raw.Y

# process manager
pm = pkd.core.SimpleProcessManager()

# smooth
sg = pkd.core.SavitzkyGolaySmoother(31)
pm.append(sg)

# peak finding
pm.append(pkd.core.MovingAveragePeakFinder(400))

# process
processed_counts = pm.run(snippedCounts)

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

f = plt.figure(figsize=(10,7))
plt.semilogy(*getplotvalues(energies, counts), 'k', linewidth=0.8, alpha=0.5, label="raw")
plt.semilogy(*getplotvalues(energies, processed_counts), 'r', linewidth=2.0, alpha=0.8, label="processed")
# plt.semilogy(*getplotvalues(energies, snippedCounts), 'r', linewidth=0.4, alpha=0.5, label="no background")
plt.semilogy(*getplotvalues(energies, background_forw), 'g', linewidth=1.0, alpha=0.8, label="background - forwards")
plt.semilogy(*getplotvalues(energies, background_back), 'c', linewidth=1.0, alpha=0.8, label="background - backwards")
plt.ylim(bottom=0.5, top=1e4)
plt.xlabel("Energy (eV)", fontsize=24)
plt.ylabel("Counts", fontsize=24)
plt.legend(fontsize=18)
plt.tick_params(axis='both', which='major', labelsize=20)
plt.tick_params(axis='both', which='minor', labelsize=20)
# spacing
plt.subplots_adjust(left=0.12, bottom=0.12, right=0.97, top=0.97, wspace=0.2, hspace=0.2)
# plt.savefig('sample.png')
plt.show()