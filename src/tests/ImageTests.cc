

#include <gtest/gtest.h>

#include <MulticamDatasetReader/core/Image.h>

using namespace MDR;

class ImageTester : public ::testing::Test{
protected:
	void SetUp() override {}
	void TearDown() override {}

};

TEST_F(ImageTester, ConstructImage){
    ASSERT_NO_FATAL_FAILURE(MDR::Image img(""));
	ASSERT_NO_FATAL_FAILURE(MDR::Image img("test.jpg"));
	ASSERT_NO_FATAL_FAILURE(MDR::Image img("error.jpg"));
	ASSERT_NO_FATAL_FAILURE(MDR::Image img("test.jpg", true));
	ASSERT_NO_FATAL_FAILURE(MDR::Image img("test.jpg", false));
}

TEST_F(ImageTester, LoadImageJPG){
	Image img("test.jpg", false, false);
	ASSERT_TRUE(img.is_loaded());
	ASSERT_FALSE(img.is_depth());
	ASSERT_NO_FATAL_FAILURE(img.matrix());
}

TEST_F(ImageTester, LoadImagePNG){
	Image img("test.png", false, false);
	ASSERT_TRUE(img.is_loaded());
	ASSERT_FALSE(img.is_depth());
	ASSERT_NO_FATAL_FAILURE(img.matrix());
}

TEST_F(ImageTester, LazyLoad){
	Image img("test.jpg", false, true);
	ASSERT_FALSE(img.is_loaded());
	ASSERT_NO_FATAL_FAILURE(img.matrix());
	ASSERT_TRUE(img.is_loaded());
}

TEST_F(ImageTester, UnloadImage){
	Image img("test.jpg", false, false);
	ASSERT_TRUE(img.is_loaded());
	ASSERT_NO_FATAL_FAILURE(img.unload());
	ASSERT_FALSE(img.is_loaded());
}