//
// Created by sewt_ma on 11/23/21.
//

#include "Device.h"

#include <MulticamDatasetReader/utils/Misc.h>
#include <MulticamDatasetReader/utils/Logging.h>
#include <MulticamDatasetReader/utils/Filesystem.h>

#include <utility>
#include <algorithm>

using namespace MDR;

Device::Device(std::string name, fs::path device_path, bool load):
	m_name(std::move(name)),
	m_device_path(std::move(device_path)),
	m_is_loaded(false){
	if(load) {
		this->load();
	}
}

void Device::load() {
	if(is_loaded()){
		return;
	}
	Log::debug("Load device from " + get_path().string());

	// check for sensors
	if(CameraSensor::check_if_sensor_exists(get_path(), false)){
		mp_sensor_rgb = std::make_unique<CameraSensor>(
				Globals::sensor_name_rgb,
				get_path(),
				false,
				true,
				true
				);
	}
	if(CameraSensor::check_if_sensor_exists(get_path(), true)){
		mp_sensor_depth = std::make_unique<CameraSensor>(
				Globals::sensor_name_depth,
				get_path(),
				true,
				true,
				true
		);
	}
	if(InertialSensor::check_if_sensor_exists(get_path(), false)){
		mp_sensor_gyro = std::make_unique<InertialSensor>(
				Globals::sensor_name_gyro,
				get_path(),
				false,
				true
				);
	}
	if(InertialSensor::check_if_sensor_exists(get_path(), true)){
		mp_sensor_accel = std::make_unique<InertialSensor>(
				Globals::sensor_name_accel,
				get_path(),
				true,
				true
		);
	}

	m_is_loaded = true;
}

DeviceMeasurements Device::get(double t) {
	DeviceMeasurements measurement;
	if(mp_sensor_rgb){
		measurement.rgb = std::make_shared<Image>(
				mp_sensor_rgb->get_active_data(t)
				);
	}
	if(mp_sensor_depth){
		measurement.depth = std::make_shared<Image>(
				mp_sensor_depth->get_active_data(t)
				);
	}
	if(mp_sensor_accel){
		measurement.accel = std::make_shared<InertiaMeasurement>(
				mp_sensor_accel->get_active_data(t)
				);
	}
	if(mp_sensor_gyro){
		measurement.gyro = std::make_shared<InertiaMeasurement>(
				mp_sensor_gyro->get_active_data(t)
				);
	}
	return measurement;
}

double Device::get_time_of_next_measurement(double t, SensorType type) {
	switch (type) {
		case SensorType::rgb:
			if(mp_sensor_rgb){
				return mp_sensor_rgb->get_time_of_next_measurement(t);
			}
			else{
				Log::error("Device has no rgb sensor");
				return -1;
			}
		case SensorType::depth:
			if(mp_sensor_depth){
				return mp_sensor_depth->get_time_of_next_measurement(t);
			}
			else{
				Log::error("Device has no depth sensor");
				return -1;
			}
		case SensorType::accel:
			if(mp_sensor_accel){
				return mp_sensor_accel->get_time_of_next_measurement(t);
			}
			else{
				Log::error("Device has no accel sensor");
				return -1;
			}
		case SensorType::gyro:
			if(mp_sensor_gyro){
				return mp_sensor_gyro->get_time_of_next_measurement(t);
			}
			else{
				Log::error("Device has no gyro sensor");
				return -1;
			}
		default:
			// find next value that is within bounds
			std::vector<double> list;
			if(mp_sensor_rgb and mp_sensor_rgb->is_in_bounds(t)){
				list.emplace_back(mp_sensor_rgb->get_time_of_next_measurement(t));
			}
			if(mp_sensor_depth and mp_sensor_depth->is_in_bounds(t)){
				list.emplace_back(mp_sensor_depth->get_time_of_next_measurement(t));
			}
			if(mp_sensor_accel and mp_sensor_accel->is_in_bounds(t)){
				list.emplace_back(mp_sensor_accel->get_time_of_next_measurement(t));
			}
			if(mp_sensor_gyro and mp_sensor_gyro->is_in_bounds(t)){
				list.emplace_back(mp_sensor_gyro->get_time_of_next_measurement(t));
			}

			if(list.empty()){
				const auto& oldest   = get_first_sampling_time();
				const auto& youngest = get_last_sampling_time();
				if(t < oldest){
					return oldest;
				}
				else{
					return youngest;
				}
			}
			else{
				return *std::min_element(list.begin(), list.end());
			}
	}
}

double Device::get_first_sampling_time(SensorType type) const {
	switch(type) {
		case SensorType::rgb:
			if(mp_sensor_rgb){
				return mp_sensor_rgb->get_first_sampling_time();
			}
			else{
				Log::error("Device has no RGB sensor");
				return -1;
			}
		case SensorType::depth:
			if(mp_sensor_depth){
				return mp_sensor_depth->get_first_sampling_time();
			}
			else{
				Log::error("Device has no Depth sensor");
				return -1;
			}
		case SensorType::accel:
			if(mp_sensor_accel){
				return mp_sensor_accel->get_first_sampling_time();
			}
			else{
				Log::error("Device has no Accel sensor");
				return -1;
			}
		case SensorType::gyro:
			if(mp_sensor_gyro){
				return mp_sensor_gyro->get_first_sampling_time();
			}
			else{
				Log::error("Device has no Gyro sensor");
				return -1;
			}
		default:
			// get min values of all available sensors
			std::vector<double> list;
			if(mp_sensor_rgb){
				list.emplace_back(mp_sensor_rgb->get_first_sampling_time());
			}
			if(mp_sensor_depth){
				list.emplace_back(mp_sensor_depth->get_first_sampling_time());
			}
			if(mp_sensor_accel){
				list.emplace_back(mp_sensor_accel->get_first_sampling_time());
			}
			if(mp_sensor_gyro){
				list.emplace_back(mp_sensor_gyro->get_first_sampling_time());
			}

			// check if there is a value
			if(list.empty()){
				Log::error("This device has no sensors");
				return -1;
			}else{
				// return min value
				return *std::min_element(list.begin(), list.end());
			}
	}
}

double Device::get_last_sampling_time(SensorType type) const {
	switch(type) {
		case SensorType::rgb:
			if (mp_sensor_rgb) {
				return mp_sensor_rgb->get_last_sampling_time();
			} else {
				Log::error("Device has no RGB sensor");
				return -1;
			}
		case SensorType::depth:
			if (mp_sensor_depth) {
				return mp_sensor_depth->get_last_sampling_time();
			} else {
				Log::error("Device has no Depth sensor");
				return -1;
			}
		case SensorType::accel:
			if (mp_sensor_accel) {
				return mp_sensor_accel->get_last_sampling_time();
			} else {
				Log::error("Device has no Accel sensor");
				return -1;
			}
		case SensorType::gyro:
			if (mp_sensor_gyro) {
				return mp_sensor_gyro->get_last_sampling_time();
			} else {
				Log::error("Device has no Gyro sensor");
				return -1;
			}
		default:// get max values of all avaiable sensors
			std::vector<double> list;
			if (mp_sensor_rgb) {
				list.emplace_back(mp_sensor_rgb->get_last_sampling_time());
			}
			if (mp_sensor_depth) {
				list.emplace_back(mp_sensor_depth->get_last_sampling_time());
			}
			if (mp_sensor_accel) {
				list.emplace_back(mp_sensor_accel->get_last_sampling_time());
			}
			if (mp_sensor_gyro) {
				list.emplace_back(mp_sensor_gyro->get_last_sampling_time());
			}

			// check if there is a value
			if (list.empty()) {
				Log::error("This device has no sensors");
				return -1;
			} else {
				// return min value
				return *std::max_element(list.begin(), list.end());
			}
	}
}
