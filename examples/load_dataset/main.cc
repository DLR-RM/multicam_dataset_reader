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
	TCLAP::CmdLine cmd("Example Load Dataset");
	TCLAP::ValueArg<std::string> argInput("i", "input", "Path to Dataset", true, "", "");
	cmd.add(argInput);
	cmd.parse(argc, argv);

	MDR::Log::set_log_level(MDR::Log::log_level_value::warn);
	MDR::Dataset dataset(argInput.getValue());

	std::cout << std::to_string(dataset.get_first_sampling_time()) << std::endl;
	std::cout << std::to_string(dataset.get_last_sampling_time())  << std::endl;

	double t   = dataset.get_first_sampling_time(MDR::SensorType::rgb);
	double end = dataset.get_last_sampling_time(MDR::SensorType::rgb);

	while(t < end){
		auto measurements = dataset.get(t);

		for(std::size_t i=0; i<dataset.device_count(); ++i) {
			cv::imshow(std::to_string(i), measurements[i].rgb->matrix());
		}
		cv::waitKey(20);

		auto next_t = dataset.get_time_of_next_measurement(t, MDR::SensorType::rgb);
		if(next_t == t){
			break;
		}
		else{
			t = next_t;
			std::cout << std::to_string(t) << std::endl;
		}
	}

	return EXIT_SUCCESS;
}