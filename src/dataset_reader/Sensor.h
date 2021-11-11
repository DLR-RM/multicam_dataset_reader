//
// Created by sewt_ma on 7/29/21.
//

#ifndef MULTICAM_DATASET_SENSOR_H
#define MULTICAM_DATASET_SENSOR_H

#include "Filesystem.h"
#include "Data.h"

#include <Eigen/Eigen>

#include <utility>
#include <optional>
#include <string>
#include <tuple>
#include <map>

namespace MD{

class Sensor{
public:
	/**
	 * Create a new sensor
	 * \param sensor_name sensor name
	 */
	explicit Sensor(std::string sensor_name);

	/**
	 * Create a new sensor and load it from the filesystem
	 * \param sensor_name sensor name
	 * \param path path to sensor directory
	 */
	Sensor(std::string sensor_name,
		   const fs::path& path);

	/**
	 * load from sensor directory
	 * \param path path to sensor directory
	 * \return True if successful
	 */
	bool load(const fs::path& path);

	/**
	 * Return name of this sensor
	 * \return sensor name
	 */
	[[nodiscard]]
	std::string name() const { return m_sensor_name; }

	/**
	 * Get the extrinsic pose of this sensor
	 * \return 4x4 pose matrix
	 */
	[[nodiscard]]
	Eigen::Matrix4d extrinsic() const { return m_extrinsic; }

	/**
	 * Get the intrinsics pose of the rgb sensor
	 * \return 3x3 calibration matrix
	 */
	[[nodiscard]]
	Eigen::Matrix3d intrinsics() const { return m_intrinsics; }

	bool has_rgb() const { return m_has_rgb; }
	bool has_depth() const { return m_has_depth; }
	bool has_accel() const { return m_has_accel; }
	bool has_gyro() const { return m_has_gyro; }

	/**
	 * Typedef for sensor frame
	 */
	using sensor_frame_t = std::tuple<
	        std::optional<RGBData>,
			std::optional<DepthData>,
			std::optional<AccelData>,
			std::optional<GyroData> >;

	/**
	 * Get all sensor frames which are considered active for the
	 * current time (via TIME_DIFFERENCE_FOR_ACTIVE)
	 * \return tuple containing active frames.
	 */
	[[nodiscard]]
	sensor_frame_t get_active_frame() const;

	/**
	 * Advance sensor iterator.
	 * \param t Advance to t. If negative, use get_next_t() value
	 */
	void advance(double t=-1);

	/**
	 *  Get the current selected time on this sensor
	 * \return time, zero at start
	 */
	[[nodiscard]]
	double get_current_t() const{ return m_current_t; }

	/**
	 * Get next time where a frame is available
	 * \return next time, infinity if noting is available
	 */
	[[nodiscard]]
	double get_next_t();

private:
	/** name of the complete sensor */
	std::string m_sensor_name;

	/** extrinsic pose */
	Eigen::Matrix4d m_extrinsic;

	/** rgb calibration matrix */
	Eigen::Matrix3d m_intrinsics;

	/** sensors **/
	bool m_has_rgb   = false;
	bool m_has_depth = false;
	bool m_has_accel = false;
	bool m_has_gyro  = false;

	/** data **/
	std::map<double, RGBData>   m_data_rgb;
	std::map<double, DepthData> m_data_depth;
	std::map<double, AccelData> m_data_accel;
	std::map<double, GyroData>  m_data_gyro;

	/** iteration data */
	double TIME_DIFFERENCE_FOR_ACTIVE = 10e-6;   // 10us
	double m_current_t = 0;
	double m_next_t    = std::numeric_limits<double>::infinity();
	std::map<double, RGBData>::iterator   m_it_rgb;
	std::map<double, DepthData>::iterator m_it_depth;
	std::map<double, AccelData>::iterator m_it_accel;
	std::map<double, GyroData>::iterator  m_it_gyro;
	double m_next_t_rgb   = std::numeric_limits<double>::infinity();
	double m_next_t_depth = std::numeric_limits<double>::infinity();
	double m_next_t_accel = std::numeric_limits<double>::infinity();
	double m_next_t_gyro  = std::numeric_limits<double>::infinity();

	/**
	 * Update times of next frames
	 */
	void update_next_t();

	/**
	 * Parse sensor.txt
	 */
	bool parse_sensor_txt(const fs::path& path);

	/**
	 * Parse frames_rgb.txt
	 */
	bool parse_frames_rgb_txt(const fs::path& path);

	/**
	 * Parse frames_depth.txt
	 */
	bool parse_frames_depth_txt(const fs::path& path);

	/**
	 * Parse accel.txt
	 */
	bool parse_accel_txt(const fs::path& path);

	/**
	 * Parse gyro.txt
	 */
	bool parse_gyro_txt(const fs::path& path);

};

}
#endif //MULTICAM_DATASET_SENSOR_H
