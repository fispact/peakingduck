import unittest
import math

import pykingduck as pkd


class SpectralDataUnitTest(unittest.TestCase):

    def test_empty_channel_hist(self):
        hist = pkd.core.SpectrumChannelBased()
        self.assertEqual([], hist.X.to_list(), "Assert length of X")
        self.assertEqual([], hist.Y.to_list(), "Assert length of Y")

    def test_empty_energy_hist(self):
        hist = pkd.core.SpectrumEnergyBased()
        self.assertEqual([], hist.X.to_list(), "Assert length of X")
        self.assertEqual([], hist.Y.to_list(), "Assert length of Y")

    def test_energy_hist(self):
        narray = pkd.core.NumericalData
        
        hist = pkd.core.SpectrumEnergyBased(
            narray([1, 2, 4, 10, 12]), 
            narray([2, 4, 9, 2])
            )
            
        self.assertEqual([1, 2, 4, 10, 12], hist.X.to_list(), "Assert length of X")
        self.assertEqual([2, 4, 9, 2], hist.Y.to_list(), "Assert length of Y")

    def test_energy_hist_snip(self):
        narray = pkd.core.NumericalData
        
        counts = narray([2, 4, 34, 54, 45, 23, 25, 10, 12])
        hist = pkd.core.SpectrumEnergyBased(
            narray([1, 2, 4, 10, 12, 23, 24, 25, 28, 30]), 
            counts)
            
        self.assertEqual(counts.snip(2).to_list(), hist.estimateBackground(range(1, 3)).to_list(), "Assert snip")

