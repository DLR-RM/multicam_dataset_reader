//
// Created by sewt_ma on 11/24/21.
//

#ifndef MULTICAMDATASETREADER_MEASUREMENT_H
#define MULTICAMDATASETREADER_MEASUREMENT_H

namespace MDR{

class Measurement{
public:
	/**
	 * Tells whether this type supports lazy loading
	 * \return True if supported, False otherwise
	 */
	virtual bool can_lazy_load() const = 0;

	/**
	 * Load data
	 */
	virtual void load() = 0;

	/**
	 * Unload data
	 */
	virtual void unload() = 0;

	/**
	 * Tell whether the data is already loaded
	 * \return True if loaded, False otherwise
	 */
	virtual bool is_loaded() const = 0;
};

}

#endif //MULTICAMDATASETREADER_MEASUREMENT_H
