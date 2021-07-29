//
// Created by sewt_ma on 2/17/21.
//

#include "DatasetReader.h"
#include "Utils.h"

#include "iostream"
#include <utility>

MD::DatasetReader::DatasetReader(const fs::path& dataset_root){
	m_dataset_root = dataset_root;

	// check path for dataset
	if(is_valid_dataset(m_dataset_root)){
		// path points to valid dataset
		load(m_dataset_root);
	}
	else{
		// path points to invalid dataset
		std::cerr << "Could not load dataset. Dataset is not valid" << std::endl;
	}
}

void MD::DatasetReader::load(const fs::path& root){

}

bool MD::DatasetReader::is_valid_dataset(const fs::path& root){
	// starting at root level.
	std::vector<std::string> f{"meta.txt"};
	if(!utils::check_for_files(root, f)){
		return false;
	}

	return true;
}
