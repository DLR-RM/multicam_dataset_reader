//
// Created by sewt_ma on 11/29/21.
//

#ifndef MULTICAMDATASETREADER_GLOBALS_H
#define MULTICAMDATASETREADER_GLOBALS_H

#include <string>

#include <MulticamDatasetReader/utils/Filesystem.h>

namespace MDR{
class Globals{
public:
	// RGB
	static std::string sensor_name_rgb;
	static std::string sensor_file_rgb;
	static std::string frames_file_rgb;
	static fs::path data_path_rgb;

	// Depth
	static std::string sensor_name_depth;
	static std::string sensor_file_depth;
	static std::string frames_file_depth;
	static fs::path data_path_depth;

	// Accel
	static std::string sensor_name_accel;
	static std::string sensor_file_accel;
	static std::string frames_file_accel;

	// Gyro
	static std::string sensor_name_gyro;
	static std::string sensor_file_gyro;
	static std::string frames_file_gyro;

public:
	Globals() = delete;
	Globals(Globals&) = delete;
	Globals(Globals&&) = delete;
};
}

#endif //MULTICAMDATASETREADER_GLOBALS_H
