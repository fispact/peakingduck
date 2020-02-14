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
    pkd.core.SavitzkyGolaySmoother(3),
    pkd.core.GlobalThresholdPeakFilter(0.005),
    # pkd.core.ChunkedThresholdPeakFilter(0.05, 5000)
])

# peak finder
pf = pkd.core.SimplePeakFinder(threshold=0.5)

# process
try:
    processed_counts = pm.run(snippedCounts)
    peaks = pf.find(processed_counts)
except:
    processed_counts = counts 
    peaks = []

for peak in peaks:
    print("{1} @ {0} eV".format(energies[peak.index], peak.value))

# plot the histogram
import peakingduck.plotting as pkdplot

f = pkdplot.PLT.figure(figsize=(10,7))
pkdplot.PLT.semilogy(*pkdplot.getplotvalues(energies, counts), 'k', linewidth=0.8, alpha=0.5, label="raw")
pkdplot.PLT.semilogy(*pkdplot.getplotvalues(energies, processed_counts), 'r', linewidth=2.0, alpha=0.8, label="processed")
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
