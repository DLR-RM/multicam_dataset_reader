//
// Created by sewt_ma on 11/11/21.
//

#include "Image.h"
#include <MulticamDatasetReader/utils/Logger>

#include <opencv2/imgcodecs.hpp>

#include <utility>

MDR::Image::Image(fs::path image_path, bool data_is_depth, bool lazy_load) : 
    m_data_path(std::move(image_path)),
    m_is_depth(data_is_depth){
    if(not lazy_load){
        // already load image into memory
        load();
    }
}

void MDR::Image::load(){
    // checks
    if(not fs::path::exists(path())){
        Logger::error("File does not exist " + path().string());
        return;
    }
    if(fs::path::is_directory(path())){
        Logger::error("Path is a directory " + path().string());
        return;
    }

    // thread safe image loading
    if(is_depth()){
        Logger::debug("Loading depth image " + path().string())
        std::unique_lock<std::mutex> _lck(m_mutex_data);
        m_data = cv::imread(path().string(), IMREAD_ANYDEPTH);
    }
    else{
        Logger::debug("Loading color/sw image " + path().string())
        std::unique_lock<std::mutex> _lck(m_mutex_data);
        m_data = cv::imread(path().string(), IMREAD_ANYCOLOR);
    }
    
    // set flag if load was successfull
    data_is_loaded = not m_data.empty();
}

void MDR::Image::unload(){
    Logger::debug("Unload file " + path().string());
    m_data = cv::Mat();
    data_is_loaded = false;
}

cv::Mat matrix() const {
    // check if image is already loaded
    if(not is_loaded()){
        Logger::debug("Lazy load of image " + path().string());
        load();
    }

    // lock data and return
    std::unique_lock<std::mutex> _lck(m_mutex_data);
    return m_data;
}