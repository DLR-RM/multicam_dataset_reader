//
// Created by sewt_ma on 11/24/21.
//

#ifndef MULTICAMDATASETREADER_INERTIAMEASUREMENT_H
#define MULTICAMDATASETREADER_INERTIAMEASUREMENT_H

#include "Measurement.h"

#include <array>

namespace MDR {

class InertiaMeasurement : Measurement{
public:
	InertiaMeasurement(double ax, double ay, double az);
	InertiaMeasurement(std::array<double, 3> data);

	bool can_lazy_load() const override { return false;	}
	void load() override;
	void unload() override;
	bool is_loaded() const override { return true; };

	std::array<double, 3> get_vector() const { return m_data; }

private:
	std::array<double, 3> m_data;
};

}
#endif //MULTICAMDATASETREADER_INERTIAMEASUREMENT_H
