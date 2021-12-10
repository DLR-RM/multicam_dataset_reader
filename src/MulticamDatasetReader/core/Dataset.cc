//
// Created by sewt_ma on 12/3/21.
//

#include "Dataset.h"
#include "Globals.h"

#include <MulticamDatasetReader/utils/Logging.h>

#include <utility>
#include <algorithm>

using namespace MDR;

MDR::Dataset::Dataset(fs::path dataset_root, bool load) :
		m_dataset_root(std::move(dataset_root)),
		m_is_loaded(false){
	if(load){
		this->load();
	}
}

void MDR::Dataset::load() {
	// check if already loaded
	if(is_loaded()){
		Log::warn("Dataset already loaded");
		return;
	}

	// check if path exists
	if(not fs::exists(get_path())){
		Log::error("Path " + get_path().string() + " does not exist");
		return;
	}

	// iterate over all directories
	const auto& dir_content = get_dirs(get_path());
	for(const auto& path : dir_content){
		Log::debug("Found possible device at " + path.string());
		m_devices.emplace_back(path.stem().string(), path, true);
	}

	// check for groundtruth
	if(fs::exists(get_path()/Globals::dataset_groundtruth)){
		mp_groundtruth = std::make_unique<GroundTruthSensor>(
			get_path()/Globals::dataset_groundtruth
		);
	}
	else{
		Log::warn("No " + Globals::dataset_groundtruth + " found");
	}
}

std::vector<DeviceMeasurements> Dataset::get(double t) {
	std::vector<DeviceMeasurements> measurements;
	std::for_each(m_devices.begin(), m_devices.end(),
				  [&measurements, t](Device& d){
		measurements.push_back(d.get(t));
	});
	return measurements;
}

double Dataset::get_first_sampling_time(SensorType type) const {
	std::vector<double> device_times;
	std::for_each(m_devices.begin(), m_devices.end(),
				  [&device_times](const Device& d){
		device_times.push_back(d.get_first_sampling_time());
	});
	return *std::min_element(device_times.begin(), device_times.end());
}

double Dataset::get_last_sampling_time(SensorType type) const {
	std::vector<double> device_times;
	std::for_each(m_devices.begin(), m_devices.end(),
				  [&device_times](const Device& d){
		device_times.push_back(d.get_last_sampling_time());
	});
	return *std::max_element(device_times.begin(), device_times.end());
}

double Dataset::get_time_of_next_measurement(double t,
											 SensorType type,
											 const std::string& device_name) {
	if(device_name.empty()) {
		std::vector<double> device_times;
		std::for_each(m_devices.begin(), m_devices.end(),
		              [&device_times, t, type](Device &d) {
			              device_times.push_back(d.get_time_of_next_measurement(t, type));
		              });
		return *std::min_element(device_times.begin(), device_times.end());
	}
	else{
		return get_device_by_name(device_name).get_time_of_next_measurement(t, type);
	}
}

std::vector<std::string> Dataset::get_device_names() const {
	std::vector<std::string> names;
	std::for_each(m_devices.begin(), m_devices.end(),
	              [&names](const Device& d){
		names.push_back(d.get_name());
	});
	return names;
}

Device &Dataset::get_device_by_name(const std::string &device_name) {
	auto it = std::find_if(m_devices.begin(), m_devices.end(),
						   [&device_name](const Device& d){
		return d.get_name() == device_name;
	});
	if(it == m_devices.end()){
		Log::error("No device " + device_name + " known");
	}
	return *it;
}
