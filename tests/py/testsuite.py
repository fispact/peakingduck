import unittest

from .numericaltests import NumericalDataUnitTest
from .spectraltests import SpectralDataUnitTest


def main():
    unittest.TextTestRunner(verbosity=3).run(unittest.TestSuite())

if __name__ == '__main__':
    unittest.main()