//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_DEVICE_H
#define MULTICAMDATASETREADER_DEVICE_H

#include "CameraSensor.h"
#include "InertialSensor.h"

#include <MulticamDatasetReader/utils/Filesystem.h>

#include <vector>

namespace MDR {

class Device {
public:
	/**
	 * Create a new device
	 * \param device_path path to device directory
	 * \param load load device
	 */
	Device(fs::path device_path, bool load=true);

	/**
	 * Load device data
	 */
	void load();

	/**
	 * Return whether this device is loaded
	 * \return True if loaded, False otherwise
	 */
	bool is_loaded() const { return m_is_loaded; }

	/**
	 * Return device path
	 * \return device path
	 */
	fs::path get_path() const { return m_device_path; }

private:
	/**
	 * Add a new vision sensor to camera list
	 * \param name sensor name
	 * \param is_depth True if depth sensor, False otherwise
	 * \param is_lazy_load True if lazy load shall be activated,
	 *                     False otherwise
	 * \param load True if shall be loaded immediately,
	 *             False otherwise
	 */
	void add_vision_sensor(const std::string& sensor_name,
						   bool is_depth,
						   bool is_lazy_load,
						   bool load);

	/**
	 * Add a new inertial sensor to list
	 * \param sensor_name sensor name
	 * \param is_accel True if accel, False if gyro
	 * \param load True if lazy load shall be activated,
	 *             False otherwise
	 */
	void add_inertial_sensor(const std::string& sensor_name,
							 bool is_accel,
							 bool load);

	/** path to device */
	fs::path m_device_path;

	/** all camera sensors */
	std::vector<CameraSensor> m_camera_sensors;

	/** all inertial sensors */
	std::vector<InertialSensor> m_inertial_sensors;

	/** whether this device is already loaded */
	bool m_is_loaded;

	////////////////////////////////////////////
	// Dataset Properties
	////////////////////////////////////////////
};

}
#endif //MULTICAMDATASETREADER_DEVICE_H
