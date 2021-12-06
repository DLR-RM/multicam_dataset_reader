//
// Created by sewt_ma on 7/30/21.
//

#include "Data.h"

#include <utility>
#include <opencv2/imgcodecs.hpp>

MD::ImgageData::ImgageData(fs::path path):m_path(std::move(path)){}

MD::RGBData::RGBData(fs::path path):ImgageData(std::move(path)){}

void MD::RGBData::load(){
	m_data = cv::imread(m_path.string(), cv::IMREAD_ANYCOLOR);
}

MD::DepthData::DepthData(fs::path path):ImgageData(std::move(path)){}

void MD::DepthData::load(){
	m_data = cv::imread(m_path.string(), cv::IMREAD_ANYDEPTH);
}

MD::VectorData::VectorData(std::vector<double> values):m_values(std::move(values)){}
