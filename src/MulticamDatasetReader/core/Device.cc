//
// Created by sewt_ma on 11/23/21.
//

#include "Device.h"

#include <MulticamDatasetReader/utils/Misc.h>
#include <MulticamDatasetReader/utils/Logging.h>
#include <MulticamDatasetReader/utils/Filesystem.h>

#include <utility>

using namespace MDR;

Device::Device(fs::path device_path, bool load):
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
		add_vision_sensor(Globals::sensor_name_rgb,
						  false,
						  true,
						  true);
	}
	if(CameraSensor::check_if_sensor_exists(get_path(), true)){
		add_vision_sensor(Globals::sensor_name_depth,
						  true,
						  true,
						  true);
	}
	if(InertialSensor::check_if_sensor_exists(get_path(), false)){
		add_inertial_sensor(Globals::sensor_name_gyro,
		                    false,
		                    true);
	}
	if(InertialSensor::check_if_sensor_exists(get_path(), true)){
		add_inertial_sensor(Globals::sensor_name_accel,
		                    true,
		                    true);
	}

	m_is_loaded = true;
}

void Device::add_vision_sensor(const std::string &sensor_name,
							   bool is_depth,
							   bool is_lazy_load,
							   bool load) {
	Log::debug("Adding " + sensor_name);
	m_camera_sensors.emplace_back(
			sensor_name,
			m_device_path,
			is_depth,
			is_lazy_load,
			load
	);
}

void Device::add_inertial_sensor(const std::string &sensor_name,
								 bool is_accel,
								 bool load) {
	Log::debug("Adding " + sensor_name);
	// check for necessary files
	m_inertial_sensors.emplace_back(
			sensor_name,
			m_device_path,
			is_accel,
			load
	);
}
