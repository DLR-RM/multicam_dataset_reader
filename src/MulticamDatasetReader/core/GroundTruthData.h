//
// created by sewt_ma on 12/10/2021
//

#ifndef MULTICAMERADATASET_GROUNDTRUTHDATA_H
#define MULTICAMERADATASET_GROUNDTRUTHDATA_H

#include "Sensor.h"
#include "Globals.h"
#include "Measurement.h"

#include <vector>

namespace MDR{

class GroundTruthData : public Measurement{
public:
    /**
     * Create new GroundTruthData
     * \param t timestamp
     * \param px position x
     * \param py position y
     * \param pz position z
     * \param qx quaternion x
     * \param qy quaternion y
     * \param qz quaternion z
     * \param qw quaternion w
     */
    GroundTruthData(double t, 
                    double px, double py, double pz, 
                    double qx, double qy, double qz, double qw);
    
    GroundTruthData(std::array<double, 8> data);

    bool can_lazy_load() const override { return false;	}
	void load() override;
	void unload() override;
	bool is_loaded() const override { return true; };

    double t() const { return m_data[0];}
    double px() const { return m_data[1];}
    double py() const { return m_data[2];}
    double pz() const { return m_data[3];}
    double qx() const { return m_data[4];}
    double qy() const { return m_data[5];}
    double qz() const { return m_data[6];}
    double qw() const { return m_data[7];}

    std::array<double, 3> position() const;
    std::array<double, 4> quaternion() const;
private:
	std::array<double, 8> m_data;
};

}

#endif // !MULTICAMERADATASET_GROUNDTRUTHDATA_H