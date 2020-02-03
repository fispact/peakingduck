import os
import peakingduck as pd


filename = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        '..', 'reference', 'spectrum0.csv')

# read in from csv file
hist_raw = pd.core.SpectrumEnergyBased()
pd.io.from_csv(hist_raw, filename)
data = hist_raw.Y

# create a process manager
pm = pd.core.SimpleProcessManager()

# this causes problems, due to Python ref counting
# don't do this until issue #6 is fixed
#pm.append(pd.core.SavitzkyGolaySmoother(3))

# this is OK, since it is from C++
pm.append(pd.core.MovingAverageSmoother(1))
pm.append(pd.core.MovingAverageSmoother(3))

# this is OK, for python implemented algorithms
# just don't delete before append
sg = pd.core.SavitzkyGolaySmoother(3)
pm.append(sg)

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

# keep anything above threshold
tc = ThresholdCut(1e3)
pm.append(tc)

# process the data
processeddata = pm.run(data).to_list()

print(processeddata)
