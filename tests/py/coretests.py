import unittest
import math

import pykingduck as pkd


class NumericalDataUnitTest(unittest.TestCase):

    def test_empty(self):
        a = pkd.core.NumericalData()
        self.assertEqual([], a.to_list(), "Assert to list")

    def test_construct_from_list(self):
        a = pkd.core.NumericalData([5, 9, 10, -2])
        self.assertEqual([5, 9, 10, -2], a.to_list(), "Assert to list")