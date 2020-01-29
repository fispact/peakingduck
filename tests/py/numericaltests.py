import unittest
import math

import pykingduck as pkd


class NumericalDataUnitTest(unittest.TestCase):

    def test_empty(self):
        a = pkd.core.NumericalData()
        self.assertEqual(0, len(a), "Assert length of list")
        self.assertEqual([], a.to_list(), "Assert to list")

    def test_empty2(self):
        a = pkd.core.NumericalData([])
        self.assertEqual(0, len(a), "Assert length of list")
        self.assertEqual([], a.to_list(), "Assert to list")

    def test_construct_from_list_constructor(self):
        a = pkd.core.NumericalData([5, 9, 10, -2])
        self.assertEqual(4, len(a), "Assert length of list")
        self.assertEqual([5, 9, 10, -2], a.to_list(), "Assert to list")

    def test_construct_from_list(self):
        a = pkd.core.NumericalData([5, 9, 10, -2])
        self.assertEqual(4, len(a), "Assert length of list")

        # change the vector and size -larger
        a.from_list([6, 10, -34, -2, 254])
        self.assertEqual(5, len(a), "Assert new length of list")
        self.assertEqual([6, 10, -34, -2, 254], a.to_list(), "Assert to new list")

        # change the vector and size again - smaller
        a.from_list([98, 23, 3])
        self.assertEqual(3, len(a), "Assert new length of list")
        self.assertEqual([98, 23, 3], a.to_list(), "Assert to new list")

    def test_construct_from_numpy_array(self):
        import numpy as np
        a = pkd.core.NumericalData(np.array([5, 9, 10, -2]))
        self.assertEqual(4, len(a), "Assert length of list")
        self.assertEqual([5, 9, 10, -2], a.to_list(), "Assert to list")

    def test_snip(self):
        data = pkd.core.NumericalData([1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21,32.4,1.2,3.4,5.2,123.3,23.2,4.1])

        expected_order0 = [1.0, 42.2, 61.4, 2.1, 4.2, 23.4, 52.32, 2.3, 213.21,32.4,1.2,3.4,5.2,123.3,23.2,4.1]
        for e, a in zip(expected_order0, data.snip(0).to_list()):
            self.assertAlmostEqual(e, a, delta=1e-12, msg="Assert 0th order")

        expected_order4 = [1.0, 
                9.599840039956527, 
                2.2325487695967374, 
                2.1, 
                1.6262270466592428, 
                2.1984699168502932, 
                1.668357848457926, 
                2.3, 
                2.440060339020095, 
                2.494169045564714, 
                1.2, 
                2.6995733396237362, 
                3.2217712295589997, 
                3.3438927139799794, 
                22.267498846621255, 
                4.1]
        for e, a in zip(expected_order4, data.snip(4).to_list()):
            self.assertAlmostEqual(e, a, delta=1e-12, msg="Assert 4th order")

        for e, a in zip(expected_order4, data.snip(range(1,5)).to_list()):
            self.assertAlmostEqual(e, a, delta=1e-12, msg="Assert 4th order")

        expected_order20 = [1.0, 
                9.599840039956527, 
                2.2325487695967374, 
                2.1, 
                1.6262270466592428, 
                1.097732439852992, 
                1.668357848457926, 
                2.3, 
                2.440060339020095, 
                2.494169045564714, 
                1.2, 
                2.6995733396237362, 
                3.2217712295589997, 
                3.3438927139799794, 
                22.267498846621255, 
                4.1]
        for e, a in zip(expected_order20, data.snip(20).to_list()):
            self.assertAlmostEqual(e, a, delta=1e-12, msg="Assert 20th order")

        for e, a in zip(expected_order20, data.snip(range(1,21)).to_list()):
            self.assertAlmostEqual(e, a, delta=1e-12, msg="Assert 20th order")
        