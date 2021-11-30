#!/bin/bash

# create environment
ENV_DIR=/tmp/cissy/MDR_env
mkdir -p ${ENV_DIR}
conan install . -if ${ENV_DIR} -g virtualenv
source ${ENV_DIR}/activate.sh

# set g++-8
export CXX=g++-8
export CC=gcc-8

# start clion
/opt/CLion/latest/bin/clion.sh . &
disown
