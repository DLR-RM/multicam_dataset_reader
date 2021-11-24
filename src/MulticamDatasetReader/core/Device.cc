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
	Log::debug("Load device from " + m_device_path.string());

	// get content of directory
	const auto list_of_dirs  = get_dirs(m_device_path);
	const auto list_of_files = get_files(m_device_path);

	// check for sensors
	for(const auto& element : list_of_files) {
		// check for a "frames_" file
		const auto& file_name = element.stem().string();
		if(file_name.rfind("frames_", 0) == 0){
			const auto sensor_name = Utility::split(file_name, '_')[1];

			// vision sensors have a subdirectory
			if(fs::exists(m_device_path/sensor_name)){
				// add as vision sensor
				add_vision_sensor(sensor_name,
								  sensor_name == "depth",
								  true,
								  true);
			}
			else{
				// add as inertial sensor
				add_inertial_sensor(sensor_name,
									true);
			}
		}
	}
	m_is_loaded = true;
}

void Device::add_vision_sensor(const std::string &sensor_name,
							   bool is_depth,
							   bool is_lazy_load,
							   bool load) {
	Log::debug("Checking " + sensor_name);

	// check for necessary files
	const auto& frames_file = m_device_path/("frames_" + sensor_name + ".txt");
	const auto& sensor_file = m_device_path/("sensor_" + sensor_name + ".txt");
	const auto& data_path   = m_device_path/sensor_name;
	if(fs::exists(frames_file)){
		if(fs::exists(sensor_file)) {
			Log::debug("Detected sensor " + sensor_name);
			m_camera_sensors.emplace_back(
					frames_file,
					data_path,
					sensor_file,
					is_depth,
					is_lazy_load,
					load
			);
		}
		else{
			Log::warn("Could not find " + sensor_file.string());
		}
	}
	else{
		Log::warn("Could not find " + frames_file.string());
	}
}

void Device::add_inertial_sensor(const std::string &sensor_name, bool load) {
	Log::debug("Checking " + sensor_name);

	// check for necessary files
	const auto& frames_file = m_device_path/("frames_" + sensor_name + ".txt");
	const auto& sensor_file = m_device_path/("sensor_" + sensor_name + ".txt");
	if(fs::exists(frames_file)){
		if(fs::exists(sensor_file)) {
			Log::debug("Detected sensor " + sensor_name);
			m_inertial_sensors.emplace_back(
					frames_file,
					sensor_file,
					load
			);
		}
		else{
			Log::warn("Could not find " + sensor_file.string());
		}
	}
	else{
		Log::warn("Could not find " + frames_file.string());
	}
}
