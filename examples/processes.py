import os
import peakingduck as pd


filename = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        '..', 'reference', 'spectrum0.csv')

# read in from csv file
hist_raw = pd.core.SpectrumEnergyBased()
pd.io.from_csv(hist_raw, filename)
energies, data = hist_raw.X, hist_raw.Y

# custom process
class ThresholdCut(pd.core.IProcess):      
    def __init__(self, threshold):
        pd.core.IProcess.__init__(self)
        self.threshold = threshold

    def go(self, data): 
        """
            Chop off anything less than the threshold
        """ 
        return data.ramp(self.threshold)


# process manager
pm = pd.core.PySimpleProcessManager(processes=[
    pd.core.MovingAverageSmoother(1),
    pd.core.SavitzkyGolaySmoother(3),
    ThresholdCut(1e3),
    pd.core.MovingAverageSmoother(3),
])

# process the data
processeddata = pm.run(data).to_list()

# very naive filters and peak finding - just an example of 
# adding and creating processes
for i, d in enumerate(processeddata):
    if d > 0.0:
        print("Peak at energy: {:.3e} eV".format((energies[i]+energies[i+1])/2.0))
