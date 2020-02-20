#!/bin/bash

if [ ${BUILD_TYPE} != "Install" ]
then
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTS=ON -DBUILD_PY_BINDINGS=ON ..
    make -j4
    cd ${TRAVIS_BUILD_DIR}
    # Run c++ and python unit tests
    ./build/bin/peakingduckcpptests
    $PY_CMD -m coverage run --source ./peakingduck -m pytest --tb=long ./tests/py/testsuite.py
else
    unset PYTHONPATH
    $PY_CMD -m pip install --user .
    pytest ./tests/py/testsuite.py
    $PY_CMD setup.py test
    # TODO: Test CMake installation
fi
