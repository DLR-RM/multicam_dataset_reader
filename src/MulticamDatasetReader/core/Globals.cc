//
// Created by sewt_ma on 11/29/21.
//

#include "Globals.h"

using namespace MDR;

std::string Globals::sensor_name_rgb = "rgb";
std::string Globals::sensor_file_rgb = "sensor_rgb.txt";
std::string Globals::frames_file_rgb = "frames_rgb.txt";
fs::path Globals::data_path_rgb      = "rgb/";

std::string Globals::sensor_name_depth = "depth";
std::string Globals::sensor_file_depth = "sensor_depth.txt";
std::string Globals::frames_file_depth = "frames_depth.txt";
fs::path Globals::data_path_depth      = "depth/";

std::string Globals::sensor_name_accel = "accel";
std::string Globals::sensor_file_accel = "sensor_accel.txt";
std::string Globals::frames_file_accel = "accel.txt";

std::string Globals::sensor_name_gyro = "gyro";
std::string Globals::sensor_file_gyro = "sensor_gyro.txt";
std::string Globals::frames_file_gyro = "gyro.txt";

std::string Globals::dataset_groundtruth = "gt.txt";