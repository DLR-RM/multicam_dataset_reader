//
// created by sewt_ma on 12/10/2021
//

#include "GroundTruthSensor.h"

#include <MulticamDatasetReader/utils/Misc.h>
#include <MulticamDatasetReader/utils/Logging.h>

#include <array>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace MDR;

MDR::GroundTruthSensor::GroundTruthSensor(fs::path data_path):
    Sensor<GroundTruthData>("Groundtruth", std::move(data_path)){
    this->load();
}

void MDR::GroundTruthSensor::load(){
    parse_gt_file();    
}

void MDR::GroundTruthSensor::parse_gt_file(){
    if(not fs::exists(get_path())){
        Log::error("Could not find " + get_path().string());
        return;
    }

    // open file
    std::ifstream fp(get_path().string());

    // read line-by-line
    std::string line;
    while(std::getline(fp, line)){
        // check for comments
        if(line.rfind('#', 0) != 0 and not line.empty()){
            // split into sections
            const auto& parts = Utility::split(line, ' ');

            // convert to doubles
            std::array<double, 8> converted;
            std::transform(parts.begin(), parts.end(),
                           converted.begin(),
                           [](const std::string& s){
                               return std::stod(s);
                           });

            // insert converted data
            this->insert(converted[0], converted);
        }
    }

    Log::debug("Loaded " + std::to_string(size()) + " items");
}