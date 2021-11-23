//
// Created by sewt_ma on 11/23/21.
//

#include "InertialSensor.h"

MDR::InertialSensor::InertialSensor(fs::path frames_file,
									fs::path sensor_file,
									bool load):
						m_frames_file_path(std::move(frames_file)),
						m_sensor_file_path(std::move(sensor_file)),
						m_is_loaded(false){
	if(load){
		this->load();
	}
}

void MDR::InertialSensor::load() {

}
