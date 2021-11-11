//
// Created by sewt_ma on 7/29/21.
//

#include <dataset_reader/DatasetReader.h>

#include <iostream>
#include <iomanip>

int main(int argc, char** argv){
	MD::DatasetReader reader("/home_local/datasets/mcd4/mcd4_s0_r0");

	while(not reader.is_at_end()){
		reader.advance();
		std::cout << std::fixed << reader.get_next_t() << std::endl;
	}
	return 0;
}

