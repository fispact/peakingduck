#!/bin/bash

if [ ${BUILD_TYPE} != "Install" ]
then
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTS=ON -DBUILD_PY_BINDINGS=ON .. || exit -1
    make || exit -1
#    make -j4 || exit -1
    cd ${TRAVIS_BUILD_DIR} || exit -1
    # Run c++ and python unit tests
    ./build/bin/peakingduckcpptests
    cppstat=$?
    $PY_CMD -m coverage run --source ./peakingduck -m pytest --tb=long ./tests/py/testsuite.py
    pystat=$?
    if [ $cppstat != 0 -o $pystat != 0 ]
    then
	exit -1
    fi
else
    unset PYTHONPATH
    $PY_CMD -m pip install --user . || exit -1
    pytest ./tests/py/testsuite.py || -1
    # TODO: Test CMake installation
fi
