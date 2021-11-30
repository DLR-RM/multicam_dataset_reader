//
// Created by sewt_ma on 11/23/21.
//

#include <MulticamDatasetReader/core/Device.h>

#include <iostream>

int main(int argc, char** argv){
	MDR::Device dev("/home_local/datasets/mcd4/mcd4_s0_r0/front");

	std::cout << std::to_string(dev.get_first_sampling_time()) << std::endl;
	std::cout << std::to_string(dev.get_last_sampling_time()) << std::endl;

	std::cout << std::endl;

	std::cout << std::to_string(dev.get_time_of_next_measurement(dev.get_first_sampling_time())) << std::endl;
	std::cout << std::to_string(dev.get_time_of_next_measurement(dev.get_first_sampling_time()) - dev.get_first_sampling_time()) << std::endl;

	return EXIT_SUCCESS;
}