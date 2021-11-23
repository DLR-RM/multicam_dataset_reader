//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_CAMERASENSOR_H
#define MULTICAMDATASETREADER_CAMERASENSOR_H

#include "Image.h"

#include <MulticamDatasetReader/utils/Filesystem.h>
#include <Eigen/Eigen>
#include <opencv2/core.hpp>

#include <list>
#include <tuple>

namespace MDR {

class CameraSensor {
public:
	/**
	 * Create a new camera sensor
	 * \param frames_file path to file containing timestamp and data mapping
	 * \param data_path path to directory containing data
	 * \param sensor_file path to file containing sensor configuration
	 * \param is_depth whether this sensor is depth or not
	 * \param is_lazy_load whether the data shall lazy load
	 * \param load load sensor
	 */
	CameraSensor(fs::path frames_file,
				 fs::path data_path,
				 fs::path sensor_file,
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

private:
	/**
	 * Parse the frames file
	 */
	void parse_frames_file();

	/**
	 * Parse the sensor file
	 */
	void parse_sensor_file();

	/** list of image references */
	std::list<std::tuple<double, Image>> m_data_list;

	/** path to frames file */
	fs::path m_frames_file_path;

	/** path to data directory */
	fs::path m_data_path;

	/** path to config file */
	fs::path m_sensor_file_path;

	/** is this sensor a depth sensor */
	bool m_is_depth;

	/** shall the data be lazy loaded */
	bool m_is_lazy_load;

	/** is the data loaded */
	bool m_is_loaded;

	/** pose of this sensor */
	Eigen::Matrix4d m_pose;

	/** intrinsics of this sensor */
	cv::Mat m_intrinsics;
};

}
#endif //MULTICAMDATASETREADER_CAMERASENSOR_H
