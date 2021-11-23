//
// Created by sewt_ma on 11/23/21.
//

#ifndef MULTICAMDATASETREADER_DEVICE_H
#define MULTICAMDATASETREADER_DEVICE_H

#include "CameraSensor.h"

#include <MulticamDatasetReader/utils/Filesystem.h>

#include <vector>

namespace MDR {

class Device {
public:
	/**
	 * Create a new device
	 * \param device_path path to device directory
	 * \param load load device
	 */
	Device(fs::path device_path, bool load=true);

	/**
	 * Load device data
	 */
	void load();

	/**
	 * Return whether this device is loaded
	 * \return True if loaded, False otherwise
	 */
	bool is_loaded() const { return m_is_loaded; }

private:
	/** path to device */
	fs::path m_device_path;

	/** all camera sensors */
	std::vector<CameraSensor> m_camera_sensors;

	/** whether this device is already loaded */
	bool m_is_loaded;
};

}
#endif //MULTICAMDATASETREADER_DEVICE_H
