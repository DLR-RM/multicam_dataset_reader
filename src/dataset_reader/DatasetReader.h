//
// Created by sewt_ma on 2/17/21.
//

#ifndef MULTICAM_DATASET_DATASETREADER_H
#define MULTICAM_DATASET_DATASETREADER_H

#include "Filesystem.h"
#include "Sensor.h"

#include <utility>
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
	explicit DatasetReader(const fs::path& dataset_root);

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
	 * Return number of sensors currently loaded
	 * \return number of sensors
	 */
	[[nodiscard]]
	std::size_t sensor_count() const { return m_sensors.size(); }

	/**
	 * Get all sensors
	 * \return vector of all sensors
	 */
	[[nodiscard]]
	std::vector<Sensor> sensors() const { return m_sensors; }

	/**
	 * Advance current time
	 * \param t advance to t. If negative, use value of get_next_t()
	 */
	void advance(double t = -1);

	/**
	 * Get current dataset time
	 * \return current time, zero if started
	 */
	[[nodiscard]]
	double get_current_t() const { return m_current_t; }

	/**
	 * Return whether the dataset has reached the end
	 * \return True if end is reached
	 */
	bool is_at_end() {
		return get_next_t() == std::numeric_limits<double>::infinity();
	}

	/**
	 * Get next t with active frame on any sensor
	 * \return next t, infinity if nothing is available
	 */
	[[nodiscard]]
	double get_next_t();

	/**
	 * Reset current dataset.
	 */
	void reset();

private:
	std::string m_dataset_root;
	bool m_is_init = false;

	// meta information
	std::string m_dataset_name;
	std::string m_dataset_description;

	// sensors
	std::vector<Sensor> m_sensors;

	// iteration
	double m_current_t = 0;
	double m_next_t    = std::numeric_limits<double>::infinity();

	/**
	 * Parse meta.txt file
	 * \param file_path path to file
	 * \return pair <Name, Description>
	 */
	std::pair<std::string, std::string> parse_meta_txt(const fs::path& file_path);


};

}
#endif //MULTICAM_DATASET_DATASETREADER_H
