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
pm = pkd.core.PySimpleProcessManager(processes=[
    pkd.core.SavitzkyGolaySmoother(3)
])

# peak finder
# pf = pkd.core.ChunkedSimplePeakFinder(threshold=0.01, nchunks=100)
pf = pkd.core.WindowPeakFinder(threshold=2.0, nouter=80, ninner=3, enforce_maximum=True)

# process
try:
    processed_counts = pm.run(counts)
    peaks = pf.find(processed_counts)
except:
    processed_counts = counts 
    peaks = []

# plot the histogram and peaks
import peakingduck.plotting as pkdplot

f, ax = pkdplot.PLT.subplots(1, figsize=(10,7))

for peak in peaks:
    print("{1} @ {0} eV".format(energies[peak.index], peak.value))
    marker = pkdplot.PLT.Line2D((energies[peak.index], energies[peak.index+1]), 
                                (peak.value, peak.value), lw=5., 
                                ls='-', marker='.', 
                                markersize=10, 
                                markerfacecolor='r', 
                                markeredgecolor='r', 
                                alpha=0.8)
    ax.add_line(marker)

pkdplot.PLT.semilogy(*pkdplot.getplotvalues(energies, counts), 'k', linewidth=0.8, alpha=0.8, label="raw")
# pkdplot.PLT.semilogy(*pkdplot.getplotvalues(energies, processed_counts), 'r', linewidth=2.0, alpha=0.8, label="processed")
# plt.semilogy(*getplotvalues(energies, snippedCounts), 'r', linewidth=0.4, alpha=0.5, label="no background")
pkdplot.PLT.semilogy(*pkdplot.getplotvalues(energies, background_forw), 'g', linewidth=1.0, alpha=0.8, label="background - forwards")
pkdplot.PLT.semilogy(*pkdplot.getplotvalues(energies, background_back), 'c', linewidth=1.0, alpha=0.8, label="background - backwards")
pkdplot.PLT.ylim(bottom=0.5, top=1e4)
pkdplot.PLT.xlabel("Energy (eV)", fontsize=24)
pkdplot.PLT.ylabel("Counts", fontsize=24)
pkdplot.PLT.legend(fontsize=18)
pkdplot.PLT.tick_params(axis='both', which='major', labelsize=20)
pkdplot.PLT.tick_params(axis='both', which='minor', labelsize=20)
# spacing
pkdplot.PLT.subplots_adjust(left=0.12, bottom=0.12, right=0.97, top=0.97, wspace=0.2, hspace=0.2)
# plt.savefig('sample.png')
pkdplot.PLT.show()
