//
// Created by sewt_ma on 11/24/21.
//

#include "InertiaMeasurement.h"

MDR::InertiaMeasurement::InertiaMeasurement(double ax, double ay, double az) {
	m_data[0] = ax;
	m_data[1] = ay;
	m_data[2] = az;
}

MDR::InertiaMeasurement::InertiaMeasurement(std::array<double, 3> data):
		m_data(data){}

void MDR::InertiaMeasurement::load() {
	// no lazy loading
}

void MDR::InertiaMeasurement::unload() {
	// not supported
}