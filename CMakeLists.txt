#########################################
# Multicam Dataset
# DLR-RMC 2021
# Marco Sewtz
#
# @Description
#
# @Disclaimer
#########################################

# TODO Check cmake versions
cmake_minimum_required(VERSION 3.10)

project(multicam_dataset
        VERSION 0.1
        LANGUAGES CXX
        )

# C++ base library for reading and fetching
add_library(multicam_dataset_reader
        src/dataset_reader/DatasetReader.cc
        src/dataset_reader/DatasetReader.h
        )