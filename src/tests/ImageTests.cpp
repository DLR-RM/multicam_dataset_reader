

#include <gtest/gtest.h>

#include <MulticamDatasetReader/core/Image.h>

class ImageTester : public ::testing::Test{
protected:
	void SetUp() override {}
	void TearDown() override {}

};

TEST_F(ImageTester, ConstructImage){
    ASSERT_NO_FATAL_FAILURE(MDR::Image img(""));
}