#!/usr/bin/env bash

# create environment
mkdir -p /tmp/md
conan install . -if /tmp/md -g virtualbuildenv
source /tmp/md/activate_build.sh

# set g++-8
export CXX=g++-8
export CC=gcc-8
