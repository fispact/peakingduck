#!/bin/bash

if [ ${BUILD_TYPE} != "Install" ]
then
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTS=ON -DBUILD_PY_BINDINGS=ON ${WINDOWS_CMAKE_ARGS} .. || exit -1
    ${MK_CMD} -j4 || exit -1
    cd ${TRAVIS_BUILD_DIR} || exit -1
    # Run c++ and python unit tests
    ./build/bin/peakingduckcpptests
    cppstat=$?
    $PY_CMD -m coverage run --source ./peakingduck -m pytest --tb=long ./tests/py/testsuite.py
    pystat=$?
    mv ./build/py/PEAKINGDUCK.cp38-win_amd64.pyd ./build/py/PEAKINGDUCK.pyd
#    cp ./build/py/PEAKINGDUCK.cp38-win_amd64.pyd . || exit -1
#    cp ./build/py/PEAKINGDUCK.cp38-win_amd64.pyd peakingduck/util/ || exit -1
    echo "Python path is: $PYTHONPATH"
    if [ $cppstat != 0 -o $pystat != 0 ]
    then
	exit -1
    fi
else
    unset PYTHONPATH
    $PY_CMD -m pip install --user . || exit -1
    $PY_CMD -m pytest ./tests/py/testsuite.py || exit -1
    # TODO: Test CMake installation
fi
