# peakingduck


### A peak finding library leveraging AI written in C++14 with python bindings

[![Build Status](https://travis-ci.org/thomasms/peakingduck.svg?branch=master)](https://travis-ci.org/thomasms/peakingduck)
[![codecov](https://codecov.io/gh/thomasms/peakingduck/branch/master/graph/badge.svg)](https://codecov.io/gh/thomasms/peakingduck)

- [About](#about)
- [Status](#status)
- [Building](#building)

About
------
Peaking identification is crucial for gamma spectroscopy and nuclear analysis. Conventional methods (although included) are not great at finding peaks in areas of low statistics and often fail for multiplet identification (overlapping peaks). A new method involving deep learning methods has been developed to improve both precision and recall of peaks. This library contains some traditional algorithms for peak identification and some neural networks used for more modern approaches. The intention is to provide further analysis in the future (peak fitting, background subtraction, etc) but for the minute just focuses on peak identification. This can also be extended (in theory) to any 1 dimension data set containing labelled peaks.

Status
------
Very much a work in progress, but is expected to have the first version (0.0.1) ready for release in 2 months.

Building
------
It is header only c++ so nothing to buld if using it in c++. If you want python bindings enabled then it needs building, as below.
```bash
git clone https://github.com/thomasms/peakingduck
cd peakingduck
mkdir build
cd build
cmake -DBUILD_PY_BINDINGS=ON ..
make -j4
```
Note: Project uses cmake (> 3.2) to build peaking duck.
