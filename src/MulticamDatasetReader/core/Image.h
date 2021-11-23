//
// Created by sewt_ma on 11/11/21.
//

#ifndef MDR_IMAGE_H
#define MDR_IMAGE_H

#include <MulticamDatasetReader/utils/Filesystem.h>

#include <opencv2/core.hpp>

#include <mutex>

namespace MDR{

class Image {
public:
    /**
     * Create a new Image
     * \param image_path path to image to load
     * \param data_is_depth whether the data can be 
     *        interpreted as depth data
     * \param lazy_load enable lazy loading
     */
	explicit
    Image(fs::path image_path, bool data_is_depth=false, bool lazy_load=true);

	/**
	 * Copy constructor
	 * \param img other image object
	 */
	Image(const Image& img);

    /**
     * Tell whether the data is depth
     * \return True if depth
     */
    bool is_depth() const { return m_is_depth; }

    /**
     * Return the path to the data
     * \return path to data
     */
    fs::path path() const { return m_data_path; }

    /**
     * Load data from disk
     */
    void load();

    /**
     * Unload data
     */
    void unload();

    /**
     * Return whether the data is loaded
     */
    bool is_loaded() const { return data_is_loaded; }

    /**
     * get image matrix
     */
    cv::Mat matrix();

private:
    /** internal data representation */
    cv::Mat m_data;

    /** is data depth data */
    bool m_is_depth;

    /** data mutex */
    std::mutex m_mutex_data;

    /** path to data */
    fs::path m_data_path;

    /** if data is already loaded */
    bool data_is_loaded = false;
};

}

#endif //MDR_IMAGE_H