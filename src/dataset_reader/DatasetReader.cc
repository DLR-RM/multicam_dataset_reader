//
// Created by sewt_ma on 2/17/21.
//

#include "DatasetReader.h"

#include "iostream"

MD::DatasetReader::DatasetReader(const fs::path& dataset_root){
	if(is_valid_dataset(dataset_root)){
		load(dataset_root);
	}
	else{
		std::cerr << "Pointed root " << dataset_root.string() << " is not a valid dataset." << std::endl;
	}
}

void MD::DatasetReader::load(const fs::path& root_path){

}

bool MD::DatasetReader::is_valid_dataset(const fs::path& dataset_root){
	return false;
}

bool MD::DatasetReader::is_init() const{
	return false;
}

void MD::DatasetReader::reset(){

}
