import unittest
import math

import pykingduck as pkd


class NumericalDataUnitTest(unittest.TestCase):

    def test_empty(self):
        a = pkd.core.NumericalData()
        # self.assertEqual(0, len(a), "Assert length of list")
        # self.assertEqual([], a.to_list(), "Assert to list")

    def _test_construct_from_list(self):
        a = pkd.core.NumericalData([5, 9, 10, -2])
        self.assertEqual(4, len(a), "Assert length of list")
        self.assertEqual([5, 9, 10, -2], a.to_list(), "Assert to list")

    def _test_construct_from_numpy_array(self):
        import numpy as np
        a = pkd.core.NumericalData(np.array([5, 9, 10, -2]))
        self.assertEqual(4, len(a), "Assert length of list")
        self.assertEqual([5, 9, 10, -2], a.to_list(), "Assert to list")
