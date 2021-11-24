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
	bool can_lazy_load() const override { return false;	}
	void load() override;
	void unload() override;
	bool is_loaded() const { return true; };

private:
	std::array<double, 3> m_data;
};

}
#endif //MULTICAMDATASETREADER_INERTIAMEASUREMENT_H
