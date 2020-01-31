import scipy.signal
import numpy as np
import math

# raw C++ bindings library
from PEAKINGDUCK.core import IProcess, NumericalData

"""
    Add custom smoothers here.

    Implement the IProcess interface
"""

class WeightedMovingAverageNative(IProcess): 
    """
        We can extend the C++ classes here
        This is an example
    """       
    def __init__(self, windowsize):
        self.windowsize = windowsize

    def __call__(self, data):
        """
            if N=1, weights=[1] -> [1/N]
            if N=2, weights=[1,1] -> [1/2, 1/2]
            if N=3, weights=[1,2,1] -> [1/4, 2/4, 1/4]
            if N=4, weights=[1,2,2,1] -> [1/6, 2/6, 2/6, 1/6]
            if N=5, weights=[1,2,3,2,1] -> [1/9, 2/9, 3/9, 2/9, 1/9]
            ....

            weights=[1,2,..., ceil(n/2), ...., 2, 1] = [1, ..., ceil(n/2)] + [ceil(n/2), ..., 1]
            weights = weights/sum(weights)
        """
        weights = list(range(1, math.ceil(self.windowsize/2)+1)) + list(range(math.floor(self.windowsize/2),0,-1))
        weights = [w/sum(weights) for w in weights]
        return NumericalData(np.convolve(data.to_list(), weights, mode='same'))

class SavitzkyGolaySmoother(IProcess):      
    def __init__(self, windowsize, order=2):
        self.windowsize = windowsize
        self.order = order

    def __call__(self, data):  
        return NumericalData(scipy.signal.savgol_filter(data.to_list(), self.windowsize, self.order))
