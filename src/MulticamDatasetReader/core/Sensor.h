//
// Created by sewt_ma on 11/24/21.
//

#ifndef MULTICAMDATASETREADER_SENSOR_H
#define MULTICAMDATASETREADER_SENSOR_H

#include "Measurement.h"

#include <MulticamDatasetReader/utils/Filesystem.h>
#include <Eigen/Eigen>

#include <type_traits>
#include <utility>
#include <string>
#include <map>

namespace MDR {

template <typename MeasurementType>
class Sensor {
	static_assert(std::is_base_of<Measurement, MeasurementType>::value,
			"Type of this Sensor must be derived from Measurement");

public:
	/**
	 * Create new sensor with given name
	 * \param sensor_name name of this sensor
	 * \param root_path root path of this sensor
	 */
	Sensor(std::string sensor_name, fs::path root_path) :
		m_name(std::move(sensor_name)),
		m_path(std::move(root_path)){}

	/**
	 * Get sensor name
	 * \return sensor name
	 */
	std::string get_name() const { return m_name; }

	/**
	 * Get the root path of this sensor
	 * \return path
	 */
	fs::path get_path() const { return m_path;}

	/**
	 * Set sensor pose
	 * \param pose 4x4 pose matrix
	 */
	void set_pose(const Eigen::Matrix4d& pose) { m_pose = pose; }

	/**
	 * Get sensor pose
	 * \return 4x4 pose matrix
	 */
	Eigen::Matrix4d get_pose() const { return m_pose; }
	/**
	 * Get reference on the active measurement for the
	 * given time. If time is out of bounds, it will be
	 * either the oldest or youngest measurement
	 * \param t requested time
	 * \return active measurement
	 */
	MeasurementType& get_active_data(double t) {
		return m_data_set[get_time_of_active_measurement(t)];
	}

	/**
	 * Get reference to requested element
	 * \param t requested time
	 * \return measurement
	 */
	MeasurementType& get(double t) { return m_data_set[t]; }

	/**
	 * Insert new measurement
	 * \param t time
	 * \param measurement measurement
	 */
	void insert(double t, const MeasurementType& measurement){
		m_data_set.emplace(t, measurement);
	}

	/**
	 * Return whether the sensor data is empty or not
	 * \return True if empty, False otherwise
	 */
	bool empty() const { return m_data_set.empty(); }

	/**
	 * Return data size
	 * \return size
	 */
	std::size_t size() const { return m_data_set.size(); }

	/**
	 * Get the sampling time of the active measurement for
	 * the given time. If time is out of bound, it will be
	 * either the oldest or youngest measurement. This
	 * function will change some internal caching fields.
	 * \param t requested time
	 * \return sampling time of active measurement
	 */
	double get_time_of_active_measurement(double t);

	/**
	 * Check the given path if this sensor type is present
	 * \param path path to check
	 * \return True if this sensor was found, False otherwise
	 */
	static bool check_if_sensor_exists(const fs::path& path){}

private:
	/** sensor name */
	std::string m_name;

	/** path */
	fs::path m_path;

	/** sensor pose */
	Eigen::Matrix4d m_pose;

	/** data set */
	using data_set_t =  std::map<double, MeasurementType>;
	data_set_t m_data_set;
};

template<typename MeasurementType>
double MDR::Sensor<MeasurementType>::get_time_of_active_measurement(double t) {
	static auto it = m_data_set.begin();

	// reset it if not valid
	if(it == m_data_set.end()){
		it = m_data_set.begin();
	}

	// check if requested time is in the past of the iterator
	if(it->first > t){
		it = m_data_set.begin();
	}

	// find the active time
	while(it != m_data_set.end()){
		auto next = std::next(it);
		if(next != m_data_set.end() and
		   next->first <= t){
			it = next;
		}
		else{
			break;
		}
	}

	return it->first;
}


}
#endif //MULTICAMDATASETREADER_SENSOR_H
