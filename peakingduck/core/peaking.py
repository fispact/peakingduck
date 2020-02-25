import numpy as np
from scipy import signal
import os
import math
import array

# raw C++ bindings library
from PEAKINGDUCK.core import IPeakFinder, \
    SimplePeakFinder, PeakInfo, NumericalData
from PEAKINGDUCK.core import window as peakwindow
from .smoothing import SavitzkyGolaySmoother

"""
    Add custom peak finders here.

    Implement the IPeakFinder interface
"""
class ChunkedSimplePeakFinder(IPeakFinder):     
    """
        Breaks the spectrum up into nchunks
        applying the threshold relative to that chunk
    """   
    def __init__(self, threshold=0.05, nchunks=10):
        self.threshold = threshold
        self.nchunks = nchunks

    def find(self, data):
        peaks = []

        pf = SimplePeakFinder(threshold=self.threshold)

        subvalues = np.array_split(data, self.nchunks)
        last_index = 0
        for sv in subvalues:
            chunk_peaks = pf.find(NumericalData(sv))
            for peak in chunk_peaks:
                k = peak.index + last_index
                peaks.append(PeakInfo(k, peak.value))

            last_index += len(sv)

        return peaks


class ScipyPeakFinder(IPeakFinder):  
    """
        Wrapper for scipy peak finder

        TODO: pass smoother to constructor not just window size
    """   
    def __init__(self, threshold=2.0, smoothsize=1001):
        self.threshold = threshold
        self.smoothsize = smoothsize

    def find(self, data, *args, **kwargs):
        smoother = SavitzkyGolaySmoother(self.smoothsize)
        lowerThreshold = smoother.go(data)*self.threshold

        # scipy uses lower and upper array thresholds - no upper threshold
        indices, _ = signal.find_peaks(data, *args, height=(np.array(lowerThreshold.to_list()), None), **kwargs)

        peaks = []
        for i in indices:
            peaks.append(PeakInfo(i, data[i]))

        return peaks

class WindowPeakFinder(IPeakFinder):
    """
        A bespoke window method peak finder
    """   
    def __init__(self, threshold=2.0, ninner=0, nouter=40, 
                    include_point=False, enforce_maximum=False, use_grad=False):
        # number of sigma
        self.threshold = threshold
        self.ninner = ninner
        self.nouter = nouter
        self.include_point = include_point
        self.enforce_maximum = enforce_maximum
        self.use_grad = use_grad

    def find(self, data, *args, **kwargs):
        """
            Takes npoints either side of bin for each bin in histogram 
            to get mean and stddev with and without that bin

            Doesn't use the gradient yet, but we should enable this
        """
        grad = None
        if self.use_grad:
            grad = data.gradient(order=1)

        start = 3
        stop = len(data)-4

        #need at least twice as many bin values as npoints
        assert len(data) > self.nouter*2

        # find peaks
        peaks = []
        for i, value in enumerate(data):
            if i < start:
                continue
            if i > stop and stop > 0:
                continue

            # compute mean + sigma with point
            values = peakwindow(data, i, nouter=self.nouter, ninner=self.ninner, includeindex=self.include_point)
            meanvalue, stddev = values.mean(), values.stddev(ddof=1)
            localthreshold = meanvalue + stddev*self.threshold 

            # first check 2 points either side are also bigger than threshold
            if (localthreshold < value) \
                and (localthreshold <= data[i+1]) \
                and (localthreshold <= data[i-1]):
                # and (localthreshold <= data[i+2]) \
                # and (localthreshold <= data[i-2]):

                if self.enforce_maximum and \
                    ((data[i+1] > value) \
                    or (data[i-1] > value)):
                    continue

                # not gradient applied
                if not grad:
                    peaks.append(PeakInfo(i, value))
                # look at first derivative to check for + -> -
                # we only want positive peaks
                else:
                    #TODO: implement using grad
                    peaks.append(PeakInfo(i, value))
            
        return peaks
