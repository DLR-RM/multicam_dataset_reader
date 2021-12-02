//
// Created by sewt_ma on 11/23/21.
//

#include <MulticamDatasetReader/core/Device.h>

#include <opencv2/highgui.hpp>

#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char** argv){
	MDR::Device dev("/home_local/datasets/mcd4/mcd4_s0_r0/front");

	double t = dev.get_first_sampling_time(MDR::SensorType::rgb);
	double e = dev.get_last_sampling_time(MDR::SensorType::rgb);

	double last_t = 0;

	MDR::DeviceMeasurements last_meas;
	while(t < e){
		auto meas = dev.get(t);

		std::cout << std::to_string(t) << std::to_string(last_t - t) << std::endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(250));

		cv::imshow("RGB", meas.rgb->matrix());
		cv::waitKey(66);

		last_meas = meas;
		last_t    = t;
		t = dev.get_time_of_next_measurement(t, MDR::SensorType::rgb);
	}


	return EXIT_SUCCESS;
}