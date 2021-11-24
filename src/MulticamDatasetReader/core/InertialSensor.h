//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_INERTIALSENSOR_H
#define MULTICAMDATASETREADER_INERTIALSENSOR_H

#include <MulticamDatasetReader/utils/Filesystem.h>

#include <list>
#include <tuple>
#include <vector>

namespace MDR {

class InertialSensor {
public:
	/**
	 * Create new inertial sensor
	 * \param frames_file path to frames file
	 * \param sensor_file path to sensor file
	 * \param load whether the data shall be loaded
	 */
	InertialSensor(fs::path frames_file, fs::path sensor_file, bool load=true);

	/**
	 * Load data
	 */
	void load();

	/**
	 * Return whether this sensor is loaded
	 * \return True if loaded, False otherwise
	 */
	bool is_loaded() const { return m_is_loaded; }

private:
	void parse_frames_file();

	/** path to frames file */
	fs::path m_frames_file_path;

	/** path to sensor file */
	fs::path m_sensor_file_path;

	/** data list */
	std::list<std::tuple<double, std::vector<double>>> m_data;

	/** is sensor loaded */
	bool m_is_loaded;
};

}
#endif //MULTICAMDATASETREADER_INERTIALSENSOR_H
