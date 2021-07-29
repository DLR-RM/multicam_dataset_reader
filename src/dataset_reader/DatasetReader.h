//
// Created by sewt_ma on 2/17/21.
//

#ifndef MULTICAM_DATASET_DATASETREADER_H
#define MULTICAM_DATASET_DATASETREADER_H

#include "Filesystem.h"

#include <string>

namespace MD{

class DatasetReader{
public:
	// define default, copy and move constructor
	DatasetReader() = default;
	DatasetReader(const DatasetReader&) = default;
	DatasetReader(DatasetReader&&) = default;

	/**
	 * Construct DatasetReader with a given root. Will check if dataset is valid.
	 * \param dataset_root Path to dataset.
	 */
	DatasetReader(const fs::path& dataset_root);

	/**
	 * Set the dataset root to a new destination.
	 * \param root_path path to root
	 */
	void load(const fs::path& root);

	/**
	 * Return current dataset root.
	 * \return Path to current dataset root
	 */
	fs::path dataset_root() const { return m_dataset_root; }

	/**
	 * Will check given path, if it is a valid dataset.
	 * \param dataset_root path to dataset
	 * \return True if valid, False otherwise
	 */
	static bool is_valid_dataset(const fs::path& root);

	/**
	 * Will return whether the dataset has been initialized.
	 * \return True if init and ready, False otherwise
	 */
	bool is_init() const { return m_is_init; }

	/**
	 * Reset current dataset.
	 */
	void reset();

private:
	fs::path m_dataset_root;
	bool m_is_init = false;
};

}
#endif //MULTICAM_DATASET_DATASETREADER_H
