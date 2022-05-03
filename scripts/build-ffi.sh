#!/bin/bash

mkdir build
cd build
cmake .. -DMOTION_BUILD_FFI=On -DCMAKE_INSTALL_PREFIX=$(PWD)
make -j 16
