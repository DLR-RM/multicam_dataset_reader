//
// Created by sewt_ma on 2/17/21.
//
#define BOOST_TEST_MODULE test_reader
#define BOOST_TEST_MAIN
#if !defined( WIN32 )
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/included/unit_test.hpp>

#include <dataset_reader/DatasetReader.h>

#include <string>

BOOST_AUTO_TEST_CASE(CreateReader) {
	std::string path = "/home/sewt_ma/projects/image-rect-test/mcd_sc01_run01";
	BOOST_TEST_PASSPOINT();
	MD::DatasetReader reader(path);
	BOOST_TEST_PASSPOINT();
}