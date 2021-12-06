//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_CAMERASENSOR_H
#define MULTICAMDATASETREADER_CAMERASENSOR_H

#include "Image.h"
#include "Sensor.h"
#include "Globals.h"

#include <MulticamDatasetReader/utils/Filesystem.h>
#include <Eigen/Eigen>
#include <opencv2/core.hpp>

#include <list>
#include <tuple>

namespace MDR {

class CameraSensor : public Sensor<Image> {
public:
	/**
	 * Create a new camera sensor
	 * \param sensor_name name of this sensor
	 * \param sensor_root_path root path of the sensor
	 * \param is_depth whether this sensor is depth or not
	 * \param is_lazy_load whether the data shall lazy load
	 * \param load load sensor
	 */
	CameraSensor(std::string sensor_name,
				 fs::path sensor_root_path,
				 bool is_depth,
				 bool is_lazy_load=true,
				 bool load=true);

	/**
	 * Load sensor
	 */
	void load();

	/**
	 * Return whether this is a depth sensor
	 * \return True if depth sensor, False otherwise
	 */
	bool is_depth() const { return m_is_depth;}

	/**
	 * Return whether the sensor is already loaded
	 * \return True if loaded, False otherwise
	 */
	bool is_loaded() const { return m_is_loaded;}

	/**
	 * Check the given path if this sensor type is present
	 * \param path path to check
	 * \return True if this sensor was found, False otherwise
	 */
	static bool check_if_sensor_exists(const fs::path &path, bool is_depth);

	/**
	 * Get intrinsics of this sensor
	 * \return k Matrix
	 */
	cv::Mat get_intrinsics() const { return m_intrinsics; }

private:
	/**
	 * Construct frames file name
	 * \return path to frames file
	 */
	fs::path get_frames_file() const {
		if(is_depth()){
			return (get_path()/Globals::frames_file_depth);
		}
		else{
			return (get_path()/Globals::frames_file_rgb);
		}
	}

	/**
	 * Construct sensor file name
	 * \return path to sensor file
	 */
	fs::path get_sensor_file() const {
		if(is_depth()){
			return (get_path()/Globals::sensor_file_depth);
		}
		else{
			return (get_path()/Globals::sensor_file_rgb);
		}
	}

	/**
	 * Construct data path
	 * \return path to data directory
	 */
	fs::path get_data_path() const {
		if(is_depth()){
			return (get_path()/Globals::data_path_depth);
		}
		else{
			return (get_path()/Globals::data_path_rgb);
		}
	}

	/**
	 * Parse the frames file
	 */
	void parse_frames_file();

	/**
	 * Parse the sensor file
	 */
	void parse_sensor_file();

	/** is this sensor a depth sensor */
	bool m_is_depth;

	/** shall the data be lazy loaded */
	bool m_is_lazy_load;

	/** is the data loaded */
	bool m_is_loaded;
	/** intrinsics of this sensor */
	cv::Mat m_intrinsics;
};

}
#endif //MULTICAMDATASETREADER_CAMERASENSOR_H
