//
// Created by sewt_ma on 11/11/21.
//

#include "Image.h"
#include <MulticamDatasetReader/utils/Logging.h>

#include <opencv2/imgcodecs.hpp>

#include <utility>

using namespace MDR;

Image::Image(fs::path image_path, bool data_is_depth, bool lazy_load) : 
    m_data_path(std::move(image_path)),
    m_is_depth(data_is_depth){
    if(not lazy_load){
        // already load image into memory
        load();
    }
}

Image::Image(const Image &img) {
	// copy info
	this->m_is_depth = img.m_is_depth;
	this->m_data_path = img.m_data_path;

	if(img.is_loaded()){
		img.m_data.copyTo(this->m_data);
	}
}

void Image::load(){
    // checks
    if(not fs::exists(path())){
        Log::error("File does not exist " + path().string());
        return;
    }
    if(fs::is_directory(path())){
        Log::error("Path is a directory " + path().string());
        return;
    }

    // thread safe image loading
    if(is_depth()){
        Log::debug("Loading depth image " + path().string());
		cv::Mat unscaled_data = cv::imread(path().string(), cv::IMREAD_ANYDEPTH);
        std::unique_lock<std::mutex> _lck(m_mutex_data);

		// Create empty Mat as 32FC1
        m_data = cv::Mat(unscaled_data.size(), CV_32FC1);
		unscaled_data.convertTo(m_data, CV_32FC1, 0.001);
    }
    else{
        Log::debug("Loading color/sw image " + path().string());
        std::unique_lock<std::mutex> _lck(m_mutex_data);
        m_data = cv::imread(path().string(), cv::IMREAD_ANYCOLOR);
    }
    
    // set flag if load was successful
    data_is_loaded = not m_data.empty();
}

void Image::unload(){
    Log::debug("Unload file " + path().string());
    m_data = cv::Mat();
    data_is_loaded = false;
}

cv::Mat Image::matrix(){
    // check if image is already loaded
    if(not is_loaded()){
        Log::debug("Lazy load of image " + path().string());
        load();
    }

    // lock data and return
    std::unique_lock<std::mutex> _lck(m_mutex_data);
    return m_data;
}