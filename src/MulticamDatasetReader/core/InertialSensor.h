//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_INERTIALSENSOR_H
#define MULTICAMDATASETREADER_INERTIALSENSOR_H

#include "InertiaMeasurement.h"
#include "Globals.h"
#include "Sensor.h"

#include <MulticamDatasetReader/utils/Filesystem.h>

#include <list>
#include <tuple>
#include <vector>

namespace MDR {

class InertialSensor : public Sensor<InertiaMeasurement> {
public:
	/**
	 * Create new inertial sensor
	 * \param sensor_name name of this sensor
	 * \param sensor_root root directory of this sensor
	 * \param is_accel True if Accel, False if Gyro
	 * \param load whether the data shall be loaded
	 */
	InertialSensor(std::string sensor_name,
				   fs::path sensor_root,
				   bool is_accel,
				   bool load=true);

	/**
	 * Load data
	 */
	void load();

	/**
	 * Return whether this sensor represent an accel or
	 * gyro
	 * \return True if accel, False if gyro
	 */
	bool is_accel() const { return m_is_accel; }

	/**
	 * Return whether this sensor is loaded
	 * \return True if loaded, False otherwise
	 */
	bool is_loaded() const { return m_is_loaded; }

	static bool check_if_sensor_exists(const fs::path &path,
									   bool is_accel);

private:
	void parse_frames_file();
	void parse_sensor_file();

	/**
	 * Construct frames file path
	 * \return path to frames file
	 */
	fs::path get_frames_file(){
		if(is_accel()){
			return (get_path()/Globals::frames_file_accel);
		}
		else{
			return (get_path()/Globals::frames_file_gyro);
		}
	}

	/**
	 * Construct sensor file path
	 * \return path to sensor file
	 */
	fs::path get_sensor_file(){
		if(is_accel()){
			return (get_path()/Globals::sensor_file_accel);
		}
		else{
			return (get_path()/Globals::sensor_file_gyro);
		}
	}

	/** path to frames file */
	fs::path m_sensor_directory;

	/** intertial type */
	bool m_is_accel;

	/** is sensor loaded */
	bool m_is_loaded;
};

}
#endif //MULTICAMDATASETREADER_INERTIALSENSOR_H
