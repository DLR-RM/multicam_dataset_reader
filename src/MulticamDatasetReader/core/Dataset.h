//
// Created by sewt_ma on 12/3/21.
//

#ifndef MULTICAMDATASETREADER_DATASET_H
#define MULTICAMDATASETREADER_DATASET_H

#include "Device.h"

#include <MulticamDatasetReader/utils/Filesystem.h>

#include <vector>

namespace MDR {

class Dataset {
public:
	/**
	 * Create a new dataset with given path
	 * \param dataset_root Path to dataset root directory
	 * \param load load dataset
	 */
	explicit
	Dataset(fs::path dataset_root, bool load=true);

	/**
	 * Get dataset root directory
	 * \return root directory
	 */
	fs::path get_path() const { return m_dataset_root; }

	/**
	 * Load dataset
	 */
	void load();

	/**
	 * Return whether this dataset is loaded
	 * \return True if loaded, False otherwise
	 */
	bool is_loaded() const { return m_is_loaded; }

	/**
	 * Return the number of devices in this dataset
	 * \return number of devices
	 */
	std::size_t device_count() const { return m_devices.size(); }

	/**
	 * Return whether this dataset ist empty
	 * \return True if empty, False otherwise
	 */
	bool empty() const { return m_devices.empty(); }

	/**
	 * Get active measurements for given time
	 * \param t requested time
	 * \return Measurements
	 */
	std::vector<DeviceMeasurements> get(double t);

	/**
	 * Get the time for the next measurement with respect to the
	 * requested time. If the time is out of bounds, it will be either
	 * the youngest or oldest measurement
	 * \param t requested time
	 * \param type requested sensor type, or all sensors
	 * \param sensor_name requested device, or all devices
	 * \return time of next measurement
	 */
	double get_time_of_next_measurement(double t,
	                                    SensorType type=SensorType::all,
										const std::string& device_name="");

	/**
	 * Get the first sampling time of any sensor
	 * \param type select sensor type, or all
	 * \return oldest time
	 */
	double get_first_sampling_time(SensorType type=SensorType::all) const;

	/**
	 * Get the last sampling time of any sensor
	 * \param type select sensor type, or all
	 * \return youngest time
	 */
	double get_last_sampling_time(SensorType type=SensorType::all) const;

	/**
	 * Get all devices in this dataset
	 * \return vector of devices
	 */
	std::vector<Device>& get_devices() { return m_devices; }

	/**
	 * Get access to device by name
	 * \param device_name requested device name
	 * \return reference to device
	 */
	Device& get_device_by_name(const std::string& device_name);

	/**
	 * Get a list of all device names
	 * \return vector of device names
	 */
	std::vector<std::string> get_device_names() const;

private:
	/** root directory of this dataset */
	fs::path m_dataset_root;

	/** all devices in this dataset */
	std::vector<Device> m_devices;

	/** Flag showing if this dataset was loaded */
	bool m_is_loaded;
};

}
#endif //MULTICAMDATASETREADER_DATASET_H
