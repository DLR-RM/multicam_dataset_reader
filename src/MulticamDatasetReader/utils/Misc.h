//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_STRINGS_H
#define MULTICAMDATASETREADER_STRINGS_H

#include <opencv2/core.hpp>
#include <Eigen/Eigen>

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace MDR {
namespace Utility {
/**
 * Split string into already created vector
 * \tparam Out vector type
 * \param s string
 * \param delim
 * \param result
*/
template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		if(not item.empty()){
			*result++ = item;
		}
	}
}


/**
* Split a string into substrings
* \param s string to split
* \param delim delimiter character
* \return vector of substrings
*/
std::vector<std::string> split(const std::string &s, char delim);

/**
* Convert to Eigen 4x4 matrix
* \param px
* \param py
* \param pz
* \param qx
* \param qy
* \param qz
* \param qw
* \return
*/
Eigen::Matrix4d create_pose_matrix(double px,
                                   double py,
                                   double pz,
                                   double qx,
                                   double qy,
                                   double qz,
                                   double qw);

/**
* Create OpenCV intrinsics matrix
* \param fx
* \param fy
* \param cx
* \param cy
* \param skew
* \return
*/
cv::Mat create_intrinsics(float fx,
                          float fy,
                          float cx,
                          float cy,
                          float skew = 0.);


}
}
#endif //MULTICAMDATASETREADER_STRINGS_H
