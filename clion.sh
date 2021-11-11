#!/bin/bash

# create environment
conan install . -if build -g virtualenv
source build/activate.sh

# set g++-8
export CXX=g++-8
export CC=gcc-8

# start clion
/opt/CLion/latest/bin/clion.sh . &
disown