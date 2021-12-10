//
// Created by sewt_ma on 11/23/21.
//

#include <MulticamDatasetReader/core/Dataset.h>
#include <MulticamDatasetReader/utils/Logging.h>

#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

int main(int argc, char** argv){
	MDR::Log::set_log_level(MDR::Log::log_level_value::warn);
	TCLAP::CmdLine cmd("Example Load Dataset");
	TCLAP::ValueArg<std::string> argInput("i", "input", "Path to Dataset", true, "", "");

	try{	
		cmd.add(argInput);
		cmd.parse(argc, argv);
	} catch(...)
	{
		MDR::Log::error("Could not parse CLI");
		return EXIT_FAILURE;
	}

	// Load dataset
	MDR::Dataset dataset(argInput.getValue());

	std::cout << std::to_string(dataset.device_count()) << " devices in dataset" << std::endl;
	for(auto& device : dataset.get_devices()){
		std::cout << device.get_name() << std::endl;
		if(device.has_rgb()){
			std::cout << "--" << device.get_rgb().get_name() << std::endl;
			std::cout << device.get_rgb().get_intrinsics() << std::endl;
			std::cout << device.get_rgb().get_pose() << std::endl;
			std::cout << "" << std::endl;
		}
		if(device.has_depth()){
			std::cout << "--" << device.get_depth().get_name() << std::endl;
		}
		if(device.has_accel()){
			std::cout << "--" << device.get_accel().get_name() << std::endl;
		}
		if(device.has_gyro()){
			std::cout << "--" << device.get_gyro().get_name() << std::endl;
		}
	}

	if(dataset.has_groundtruth()){
		std::cout << "--" << dataset.get_groundtruth().get_name() << std::endl;
		std::cout << std::to_string(dataset.get_groundtruth().size()) << std::endl;
	}

	return EXIT_SUCCESS;
}