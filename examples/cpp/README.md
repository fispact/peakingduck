# Using peakingduck from C++

This directory contains an example of how you can used peakingduck as
a header-only library. It assumes that peakingduck has been installed
somewhere on your system using CMake. This can be done by executing
the following, starting in the top-level 

```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=~/.local ..
make
make install
```

From this directory, you can then run

```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=~/.local ..
make
./processes
```
