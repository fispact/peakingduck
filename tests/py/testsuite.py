import unittest

from .coretests import NumericalDataUnitTest


def main():
    unittest.TextTestRunner(verbosity=3).run(unittest.TestSuite())

if __name__ == '__main__':
    unittest.main()