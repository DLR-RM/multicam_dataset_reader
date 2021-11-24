//
// Created by sewt_ma on 11/24/21.
//

#include <gtest/gtest.h>

#include <MulticamDatasetReader/core/Sensor.h>

using namespace MDR;

class SensorTester : public ::testing::Test{
protected:
	void SetUp() override {

	}
	void TearDown() override {}

};

typedef MDR::Sensor<int> TestSensorType;

TEST_F(SensorTester, Creation){
	ASSERT_NO_FATAL_FAILURE(TestSensorType t("Test"));
}

TEST_F(SensorTester, InsertData){
	TestSensorType t("Test");
	ASSERT_NO_FATAL_FAILURE(t.insert(0, 0));
	ASSERT_NO_FATAL_FAILURE(t.insert(1, 10));
	ASSERT_NO_FATAL_FAILURE(t.insert(2, 20));
	ASSERT_NO_FATAL_FAILURE(t.insert(3, 30));

	ASSERT_FLOAT_EQ(0, t.get(0));
	ASSERT_FLOAT_EQ(10, t.get(1));
	ASSERT_FLOAT_EQ(20, t.get(2));
	ASSERT_FLOAT_EQ(30, t.get(3));

	ASSERT_FLOAT_EQ(0, t.get_time_of_active_measurement(0.0));
	ASSERT_FLOAT_EQ(0, t.get_time_of_active_measurement(0.1));
	ASSERT_FLOAT_EQ(0, t.get_time_of_active_measurement(0.5));
	ASSERT_FLOAT_EQ(0, t.get_time_of_active_measurement(0.9));
	ASSERT_FLOAT_EQ(0, t.get_time_of_active_measurement(-1.));

	ASSERT_FLOAT_EQ(2, t.get_time_of_active_measurement(2.0));
	ASSERT_FLOAT_EQ(2, t.get_time_of_active_measurement(2.2));

	ASSERT_FLOAT_EQ(3, t.get_time_of_active_measurement(5.0));

	ASSERT_EQ(0, t.get_active_data(0));
	ASSERT_EQ(0, t.get_active_data(0.1));
	ASSERT_EQ(0, t.get_active_data(0.5));
	ASSERT_EQ(0, t.get_active_data(0.9));
	ASSERT_EQ(0, t.get_active_data(-1.));
}