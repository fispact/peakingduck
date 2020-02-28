import os
import re
import glob
import sys
import platform
import subprocess
from shutil import copy

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion
from distutils import sysconfig


VERSION = ""
with open(".VERSION", 'rt') as vfile:
    VERSION = [ line.strip("\n") for line in vfile.readlines() ][0]

print("PeakingDuck version: {}".format(VERSION))

requirements_install = []
with open("requirements.txt", 'rt') as rfile:
    requirements_install = [ line.strip("\n") for line in rfile.readlines() ] 

requirements_dev = []
with open("requirements.dev.txt", 'rt') as rfile:
    requirements_dev = [ line.strip("\n") for line in rfile.readlines() ] 

"""
    CMake is used with pybind11 to make 'raw' C++ bindings
    in PEAKINGDUCK library. These bindings are then wrapped
    and aliased in a python library, peakingduck, which should
    be imported via:

    ```
       import peakingduck as pd

       data = pd.core.NumericalData([1,4,2,...])
       ...
    ```

    Not via the raw bindings as below:

    ```
        # this will work but cause issues with higher level python implementations
       import PEAKINGDUCK as pd

       data = pd.core.NumericalData([1,4,2,...])
       ...
    ```
"""
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.2':
                raise RuntimeError("CMake >= 3.2 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        if not os.path.exists(extdir):
            os.makedirs(extdir)

        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable,
                      '-DPYTHON_INCLUDE_DIR=' + sysconfig.get_python_inc()]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            cmake_args.append('-DCMAKE_MAKE_PROGRAM=mingw32-make')
            #if sys.maxsize > 2**32:
            #    cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j8']

        # do not build tests for python install
        cmake_args += ['-DBUILD_TESTS=OFF']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

        # Move from build temp to final position
        # NOTE: this is a bad way to do this - we should setup CMake and this setup.py file
        # to use library paths. Here we have to copy from the temp dir to the build dir
        # because we are using a cutom library path i.e build/py and we have not correctly
        # setup the CMake extension. This seems to work for now. 
        # Changes to cmake will require changes here!!!
        for sfile in glob.glob(r'{}*.so'.format(os.path.join(self.build_temp, 'py', ext.name))):
            copy(sfile, os.path.join(extdir))


# Do the setup
setup(name='peakingduck',
    version=VERSION,
    description='A peak finding library leveraging AI written in C++14 with python bindings',
    long_description='',
    url='https://github.com/thomasms/peakingduck',
    author='Thomas Stainer',
    author_email='me@physicstom.com',
    license='MIT',
    packages=[
        'peakingduck',
        'peakingduck.util',
        'peakingduck.core',
        'peakingduck.io',
        'peakingduck.plotting',
    ],
    install_requires=requirements_install,
    python_requires='>=3',
    setup_requires=[
        'pytest-runner',
    ],
    tests_require=requirements_install + requirements_dev,
    test_suite='tests.testsuite',
    ext_modules=[CMakeExtension('PEAKINGDUCK')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False)
