//
// created by sewt_ma on 12/10/2021
//

#include "GroundTruthData.h"

#include <utility>
#include <algorithm>

using namespace MDR;

MDR::GroundTruthData::GroundTruthData(double t, 
                    double px, double py, double pz, 
                    double qx, double qy, double qz, double qw):
                    m_data({t, px, py, pz, qx, qy, qz ,qw}){}

MDR::GroundTruthData::GroundTruthData(std::array<double, 8> data):
    m_data(std::move(data)){}

std::array<double, 3> MDR::GroundTruthData::position() const{
    std::array<double, 3> position_array;
    std::copy(m_data.begin() + 1, m_data.end() + 4, position_array.begin());
    return position_array;
}

std::array<double, 4> MDR::GroundTruthData::quaternion() const{
    std::array<double, 4> quaternion_array;
    std::copy(m_data.begin() + 4, m_data.end(), quaternion_array.begin());
    return quaternion_array;
}

void MDR::GroundTruthData::load() {
    // nothing to implement
}

void MDR::GroundTruthData::unload() {
    // nothing to implement
}