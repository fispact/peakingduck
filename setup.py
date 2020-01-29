from setuptools import setup


VERSION = ""
with open(".VERSION", 'rt') as vfile:
    VERSION = [ line.strip("\n") for line in vfile.readlines() if line.strip("\n")][0]

print(VERSION)
setup(name='peakingduck',
      version=VERSION,
      description='A peak finding library leveraging AI written in C++14 with python bindings',
      url='https://github.com/thomasms/peakingduck',
      author='Thomas Stainer',
      author_email='me@physicstom.com',
      license='MIT',
      packages=[
          'peakingduck'
      ],
      install_requires=[
          'numpy'
      ],
      python_requires='>=3',
      setup_requires=[
            'pytest-runner',
      ],
      tests_require=[
            'pytest',
            'numpy',
      ],
      test_suite='tests.testsuite',
      zip_safe=False)