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
		m_path(std::move(root_path)),
		m_pose(Eigen::Matrix4d::Identity()){}

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
	double get_time_of_active_measurement(double t){
		return get_iterator_to_active_measurement(t)->first;
	}

	/**
	 * Get the next sampling time with respect to the
	 * given time. If this time is out of bounds, it
	 * will be either the oldest or youngest measurement
	 * \param t requested time
	 * \return sampling time of active measurement
	 */
	double get_time_of_next_measurement(double t){
		return get_iterator_to_next_measurement(t)->first;
	}

	/**
	 * Check the given path if this sensor type is present
	 * \param path path to check
	 * \return True if this sensor was found, False otherwise
	 */
	static bool check_if_sensor_exists(const fs::path& path){}

	/**
	 * Get reference on the active measurement for the
	 * given time. If time is out of bounds, it will be
	 * either the oldest or youngest measurement
	 * \param t requested time
	 * \return active measurement
	 */
	MeasurementType& get_active_data(double t) {
		return m_data_set.at(get_time_of_active_measurement(t));
	}

	/**
	 * Get reference to requested element
	 * \param t requested time
	 * \return measurement
	 */
	MeasurementType& get(double t) { return m_data_set.at(t); }

	/**
	 * Insert new measurement
	 * \param t time
	 * \param measurement measurement
	 */
	void insert(double t, const MeasurementType& measurement){
		m_data_set.emplace(t, measurement);

		// reset cache to start
		m_cached_iterator = m_data_set.begin();
	}

	/**
	 * Get first sampling time of this sensor
	 * \return oldest time
	 */
	double get_first_sampling_time() const {
		return m_data_set.begin()->first;
	}

	/**
	 * Get last sampling time of this sensor
	 * \return youngest time
	 */
	double get_last_sampling_time() const {
		return m_data_set.rbegin()->first;
	}

	/**
	 * Return whether the given time is withing the timespan
	 * of this sensor. This does not mean that a $t>t_{max}$
	 * is not active.
	 * \param t time to check
	 * \return True if within bounds, False otherwise
	 */
	bool is_in_bounds(double t) const {
		if(t < get_first_sampling_time()){
			return false;
		}
		if( t > get_last_sampling_time()){
			return false;
		}
		return true;
	}

protected:
	using data_set_t = std::map<double, MeasurementType>;

	/**
	 * Get an iterator to the active measurement of requested time
	 * \param t requested time
	 * \return iterator to active element
	 */
	typename data_set_t::iterator get_iterator_to_active_measurement(double t);

	/**
	 * Get an iterator to the next active measurement with respect to
	 * the requested time
	 * \param t requested time
	 * \return iterator to next element
	 */
	typename data_set_t::iterator get_iterator_to_next_measurement(double t);

	/**
	 * Set extrinsic pose of this sensor
	 * \param pose 4x4 pose matrix
	 */
	void set_pose(Eigen::Matrix4d& pose) { m_pose = pose; }

private:
	/** sensor name */
	std::string m_name;

	/** path */
	fs::path m_path;

	/** sensor pose */
	Eigen::Matrix4d m_pose;

	/** data set */
	data_set_t m_data_set;

	/** iterator cache */
	typename data_set_t::iterator m_cached_iterator;
};

template<typename MeasurementType>
typename MDR::Sensor<MeasurementType>::data_set_t::iterator
Sensor<MeasurementType>::get_iterator_to_active_measurement(double t) {
	auto& it = m_cached_iterator;

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

	return it;
}


template<typename MeasurementType>
typename MDR::Sensor<MeasurementType>::data_set_t::iterator
Sensor<MeasurementType>::get_iterator_to_next_measurement(double t) {
	auto it = get_iterator_to_active_measurement(t);

	// check if requested time was before the active time
	if(t < it->first){
		// then the first element is the next
		return it;
	}

	auto next = std::next(it);
	if(next != m_data_set.end()){
		return next;
	}
	else{
		return it;
	}
}

}
#endif //MULTICAMDATASETREADER_SENSOR_H
