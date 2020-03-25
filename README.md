# peakingduck


### A peak finding library leveraging AI written in C++14 with python bindings

[![Build Status](https://api.travis-ci.com/fispact/peakingduck.svg?branch=master)](https://travis-ci.com/fispact/peakingduck)
[![codecov](https://codecov.io/gh/fispact/peakingduck/branch/master/graph/badge.svg)](https://codecov.io/gh/fispact/peakingduck)
[![HitCount](http://hits.dwyl.io/fispact/peakingduck.svg)](http://hits.dwyl.io/fispact/peakingduck)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

- [About](#about)
- [Motivation](#motivation)
- [Status](#status)
- [Building](#building)
- [PyPi](#pypi)

About
------
Peaking identification is crucial for gamma spectroscopy and nuclear analysis. Conventional methods (although included) are not great at finding peaks in areas of low statistics and often fail for multiplet identification (overlapping peaks). A new method involving deep learning methods has been developed to improve both precision and recall of peaks. This library contains some traditional algorithms for peak identification and some neural networks used for more modern approaches. The intention is to provide further analysis in the future (peak fitting, background subtraction, etc) but for the minute just focuses on peak identification. This can also be extended (in theory) to any 1 dimension data set containing labelled peaks.

##### Example - SNIP estimation
![SNIP](https://github.com/fispact/peakingduck/blob/master/figures/sample.png)

Motivation
------
This project is based on a prototype written in Python. We have now began porting to c++/python using pybind11. Most of the algorithms remain in python. We define some underlying interfaces and datastructures in c++ for static safety and performance (no benchmarking done yet) with the ability to extend in both c++ and python.
- **Why write this in c++ if you are just binding it to python?** We want core algorithms in c++ for performance and static typing. We want the power of python to play with algorithms and extend the base, especially making use of machine learning and AI.
- **Why not just use numpy and everything in python?** Numpy is great and we don't want to replace it, but there is no numpy in c++ (see question above). Eigen is the closest thing and is pretty good at optimising (using expression templates and SIMD). Whilst we could have used numpy for 80% of the algorithms, for the remaining 20% we cannot import numpy on the c++ side. Since everything is based on the NumericalData type (Eigen 1D array) it provides 100% consistency and easily maps to numpy. Plus c++ people can use parts of the library without having to worry about python at all.
- **Can I just use the c++ parts and ignore python?** You can, but it would not do much. Most of the power is in the algorithms implemented in python, particularly the pre-trained models. In the future it would be great if everything could be ported from python to c++, but this may not happen.

Status
------
Very much a work in progress, but is expected to have the first version (0.0.1) ready for release in 2 months.

Building
------
It is header only C++ so nothing to build (only unit tests) if using it in C++. 
If you want python bindings enabled (which I hope you do) then it needs building (default will build them), as below.
```bash
git clone --recursive -j8 https://github.com/fispact/peakingduck
cd peakingduck
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_PY_BINDINGS=ON ..
make -j4
```
Or pip should work too:
```bash
git clone --recursive -j8 https://github.com/fispact/peakingduck
cd peakingduck
pip3 install --user .
```

Note: Project uses cmake (> 3.2) to build peaking duck.

PyPi
------
The peakingduck repo has a placeholder on PyPi as ```peakingduck``` but is yet to have an official release. Once version 0.1 is ready do:

```bash
pip3 install --user peakingduck
```
