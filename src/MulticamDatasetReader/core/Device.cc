//
// Created by sewt_ma on 11/23/21.
//

#include "Device.h"

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

	// check for vision sensors
	for(const auto& element : get_dirs(m_device_path)){
		const auto& sensor_name = element.stem().string();
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
						false,
						true,
						true
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

	m_is_loaded = true;
}
