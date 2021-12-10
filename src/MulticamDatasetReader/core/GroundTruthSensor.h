//
// created by sewt_ma on 12/10/2021
//

#ifndef MULTICAMERADATASET_GROUNDTRUTHSENSOR_H
#define MULTICAMERADATASET_GROUNDTRUTHSENSOR_H

#include "Sensor.h"
#include "Globals.h"
#include "GroundTruthData.h"

#include <MulticamDatasetReader/utils/Filesystem.h>

namespace MDR{

class GroundTruthSensor : public Sensor<GroundTruthData>{
public:
    /**
     * \brief Construct a new GroundTruthSensor object
     * 
     * \param data_path path to ground truth file
     */
    explicit
    GroundTruthSensor(fs::path data_path);

    /**
     * \brief load data from harddisk
     */
    void load();

private:
    void parse_gt_file();
};

}

#endif // !MULTICAMERADATASET_GROUNDTRUTHSENSOR_H