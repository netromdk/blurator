blurator
========

Blurator blurs faces and plates in pictures very efficiently.

Requirements
============

A C++11 compliant compiler (GCC 4.7+, Clang 3.3+ etc.), CMake 3+,
Qt 5.3+, OpenCV 2.4+, and Python 3+.

Compilation
===========

To compile the source code and link the binaries do the following:

1. Extract source and go into the diretory.
2. `mkdir build`
3. `cd build`
4. `cmake ..`
5. `make`

This produces the release version binary in the *bin* folder: blurator

For debugging purposes swap out the `cmake ..` command with `cmake
-DCMAKE_BUILD_TYPE=Debug ..`.

Testing
=======

In order to run our test suite you must do the following:

1. `cmake -DENABLE_TESTING=YES ..`
2. `make`
3. `make test`

It will run all tests and show the results.

Usage
=====

```
Usage: blurator [options] paths..
Blur license plates and faces.

Supported image formats: jpg, png, tiff

Options:
  -h, --help     Displays this help.
  -v, --version  Displays version information.
  -f, --faces    Detect faces.
  -p, --plates   Detect license plates.
  -y, --yes      Automatically reply "yes" to all questions.
  -n, --no       Automatically reply "no" to all questions.
  --verbose      Shows extra information.
  --no-backup    Don't store a backup of the original image.

Arguments:
  paths          Paths to images.
```
