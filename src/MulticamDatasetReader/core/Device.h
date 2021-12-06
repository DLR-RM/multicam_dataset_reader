//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_DEVICE_H
#define MULTICAMDATASETREADER_DEVICE_H

#include "Image.h"
#include "InertiaMeasurement.h"
#include "CameraSensor.h"
#include "InertialSensor.h"

#include <MulticamDatasetReader/utils/Filesystem.h>

#include <vector>
#include <memory>

namespace MDR {

enum class SensorType{
	rgb,
	depth,
	accel,
	gyro,
	all
};

struct DeviceMeasurements{
	std::shared_ptr<Image> rgb;
	std::shared_ptr<Image> depth;
	std::shared_ptr<InertiaMeasurement> accel;
	std::shared_ptr<InertiaMeasurement> gyro;

	bool has_rgb() const { return (bool) rgb; }
	bool has_depth() const { return (bool) depth; }
	bool has_accel() const { return (bool) accel; }
	bool has_gyro() const { return (bool) gyro; }
};

class Device {
public:
	/**
	 * Create a new device
	 * \param name name of the device
	 * \param device_path path to device directory
	 * \param load load device
	 */
	Device(std::string name, fs::path device_path, bool load=true);

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

	/**
	 * Return device name
	 * \return device name
	 */
	std::string get_name() const { return m_name; }

	/**
	 * Return whether this device has a RGB sensor
	 * \return True if available, False otherwise
	 */
	bool has_rgb() const { return not mp_sensor_rgb; }

	/**
	 * Return whether this device has a depth sensor
	 * \return True if available, False otherwise
	 */
	bool has_depth() const { return not mp_sensor_depth; }

	/**
	 * Return whether this device has a accel sensor
	 * \return True if available, False otherwise
	 */
	bool has_accel() const { return not mp_sensor_accel; }

	/**
	 * Return whether this device has a gyro sensor
	 * \return True if available, False otherwise
	 */
	bool has_gyro() const { return not mp_sensor_gyro; }

	/**
	 * Get active measurements for given time
	 * \param t requested time
	 * \return Measurements
	 */
	DeviceMeasurements get(double t);

	/**
	 * Get the time for the next measurement with respect to the
	 * requested time. If the time is out of bounds, it will be either
	 * the youngest or oldest measurement
	 * \param t
	 * \param type
	 * \return
	 */
	double get_time_of_next_measurement(double t,
										SensorType type=SensorType::all);

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
	 * Return whether the given time is within the time bounds
	 * of all sensors
	 * \param t time
	 * \return True if within bounds, False otherwise
	 */
	bool in_bounds(double t) const{
		if(t < get_first_sampling_time()){
			return false;
		}
		if(t > get_last_sampling_time()){
			return false;
		}
		return true;
	}

private:
	/** device name */
	std::string m_name;

	/** path to device */
	fs::path m_device_path;

	/** all camera sensors */
	std::unique_ptr<CameraSensor> mp_sensor_rgb;
	std::unique_ptr<CameraSensor> mp_sensor_depth;

	/** all inertial sensors */
	std::unique_ptr<InertialSensor> mp_sensor_accel;
	std::unique_ptr<InertialSensor> mp_sensor_gyro;

	/** whether this device is already loaded */
	bool m_is_loaded;

	////////////////////////////////////////////
	// Dataset Properties
	////////////////////////////////////////////
};

}
#endif //MULTICAMDATASETREADER_DEVICE_H
