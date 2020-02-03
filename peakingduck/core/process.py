# raw C++ bindings library
from PEAKINGDUCK.core import IProcessManager

"""
    Add other processes and/or process managers here.
"""

class PySimpleProcessManager(IProcessManager):
    """
        Instead of using the C++ SimpleProcessManager
        we make a python native version to avoid 
        ref counting issues

        Easy to extend also.
    """
    def __init__(self, processes=None):
        self.processes = []
        if processes:
            self.processes = processes
    
    def append(self, process):
        self.processes.append(process)
    
    def __len__(self):
        return len(self.processes)

    def __item__(self, i):
        return self.processes[i]

    def run(self, data):
        newdata = data
        for p in self.processes:
            newdata = p.go(newdata)
        return newdata