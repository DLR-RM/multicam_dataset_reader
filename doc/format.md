# Dataset Format Description
This file shall describe the format used for this dataset. 

## Naming convention
In this document we will use a defined naming convention.

- `shall` defines that some condition **has** to be hold **without** any exception.
This is a strict requirement.
It's negation is `shall not`.

- `should` defines that some condition **has** to be hold **with** some exception given a good reasoning. 
This is a soft requirement.

- `run` or `dataset run` describes the complete set of all data acquired and processed during **one** experiment.

- `root` or `dataset root` can be used interchangeably.
It describes the top level of a given dataset run. 


## Directory Structure
The toplevel directory of every dataset run is the `root`.
It contains meta information on the `run` and lists all sensors.
The structure is as follows
```text
root
 |
 |- meta.txt
 |- <Sensor0>/
 |- <Sensor1>/
 ...
```

Each Sensor sub-directory contains the information and the data of this specific device acquired during the `run`.
It shall contain all data and a mapping of timestamp to file.
An example for a camera sensor directory:
```text
Sensor
  |
  |- sensor.txt
  |- rgb/
  |   |
  |   |- 0.jpg
  |   |- 1.jpg
  |   ...
  |- depth/
      |
      |- 0.exr
      |- 1.exr
      ...
```

## File Formats

### meta.txt
This file shall contain a unique name of this dataset run and a short description of what can be seen in the data.

