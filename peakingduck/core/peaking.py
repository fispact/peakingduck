import numpy as np
import os
import math
import array

# raw C++ bindings library
from PEAKINGDUCK.core import IPeakFinder, PeakInfo, NumericalData

"""
    Add custom peak finders here.

    Implement the IPeakFinder interface
"""

class SimplePeakFinder(IPeakFinder):
    """
        Identify peaks using global threshold
    """
    def __init__(self, threshold=0.05):
        self.threshold = threshold

    def find(self, data):
        max_value = data.maxCoeff()

        # find consecutive bins for a peak group
        peak_groups = []
        last_index = -1
        local_peak = []
        for i, entry in enumerate(data):
            if entry >= max_value*self.threshold:
                if last_index + 1 < i and last_index >=0:
                    peak_groups.append(local_peak)
                    local_peak = []
                local_peak.append(i)
                last_index = i
        if local_peak:
            peak_groups.append(local_peak)

        peaks = []
        # find maximum in each group
        for g in peak_groups:
            max_value = -1.0
            max_index = -1
            for i in g:
                if data[i] > max_value:
                    max_value = data[i]
                    max_index = i

            peaks.append(PeakInfo(max_index, data[max_index]))
            
        return peaks

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
