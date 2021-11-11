//
// Created by sewt_ma on 7/30/21.
//

#ifndef MULTICAM_DATASET_DATA_H
#define MULTICAM_DATASET_DATA_H

#include "Filesystem.h"

#include <opencv2/core.hpp>

#include <vector>

namespace MD{

class ImgageData{
public:
	explicit ImgageData(fs::path path);

	/**
	 * Load data
	 */
	virtual void load() = 0;

	/**
	 * Get data
	 * \return
	 */
	cv::Mat get(){
		if(!is_loaded()){
			load();
		}
		return m_data;
	};

	/**
	 * Return whether the data is already loaded
	 * \return True if loaded
	 */
	bool is_loaded() const { return !m_data.empty(); };

protected:
	cv::Mat m_data;
	fs::path m_path;
};

class RGBData : public ImgageData {
public:
	explicit RGBData(fs::path path);
	void load() override;
};

class DepthData : public ImgageData {
public:
	explicit DepthData(fs::path path);
	void load() override;
};

class VectorData{
public:
	explicit VectorData(std::vector<double> values);

	std::vector<double> get() { return m_values; }

private:
	std::vector<double> m_values;
};

using AccelData = VectorData;
using GyroData  = VectorData;

}

#endif //MULTICAM_DATASET_DATA_H
