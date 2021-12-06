//
// Created by sewt_ma on 11/23/21.
//

#include "Misc.h"

std::vector<std::string> MDR::Utility::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}
Eigen::Matrix4d MDR::Utility::create_pose_matrix(double px,
										double py,
										double pz,
										double qx,
										double qy,
										double qz,
										double qw) {
	Eigen::Matrix4d pose = Eigen::Matrix4d::Identity();

	// Quaternions
	Eigen::Quaterniond quat(qw, qx, qy, qz);
	pose.block(0,0,3,3) = quat.matrix();

	// add translation
	pose(0,3) = px;
	pose(1,3) = py;
	pose(2,3) = pz;

	return pose;
}

cv::Mat MDR::Utility::create_intrinsics(float fx, float fy, float cx, float cy, float skew) {
	cv::Mat k = cv::Mat::eye(3,3,CV_32FC1);
	k.at<float>(0,0) = fx;
	k.at<float>(1,1) = fy;
	k.at<float>(0,2) = cx;
	k.at<float>(1,2) = cy;
	k.at<float>(0,1) = skew;
	return k;
}
