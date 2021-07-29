//
// Created by sewt_ma on 7/29/21.
//

#include <dataset_reader/DatasetReader.h>
#include <dataset_reader/Filesystem.h>

#include <iostream>

int main(int argc, char** argv){
	fs::path path = "/home/sewt_ma/projects/image-rect-test/mcd_sc01_run01";
	MD::DatasetReader reader(path);

	std::cout << "end" << std::endl;
	return 0;
}