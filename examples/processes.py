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
# comment it out to find out
#pm.append(pd.core.SavitzkyGolaySmoother(3))

# this also causes issues
#sg = pd.core.SavitzkyGolaySmoother(3)
#pm.append(sg)
#del sg

# this is OK, since it is fro C++
pm.append(pd.core.MovingAverageSmoother(1))
pm.append(pd.core.MovingAverageSmoother(3))

# this is OK, for python implemented algorithms
# just don't delete before append
sg = pd.core.SavitzkyGolaySmoother(3)
pm.append(sg)

# process the data
processeddata = pm.run(data).to_list()

print(processeddata)
