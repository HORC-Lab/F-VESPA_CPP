# OOP C++ F-VESPA
In this repository, the real-time and kinematic-based foot-strike detection algorithm F-VESPA is implemented in OOP C++.

## Overview
 ### components (Most Important)
This folder contains the definition of the "ButterworthFilter" and "FootStrikeDetector" classes. 
The "ButterworthFilter" class implements a discrete-time second order Butterworth (digital) filter of specific cutoff and sampling frequencies.
The "FootStrikeDetector" class implements the real-time kinematic-based foot-strike detection algorithm F-VESPA.  

#### implementation
Definition and analysis of the member functions included in the GaitMonitor class.



 ### GaitMonitor_tests (Second Most Important)
This folder contains three different tests of the implemented algorithm, each contained in a distinct subfolder.
#### shared_mem_GaitMonitor_tests
This test is implementing the real-time kinematic-based foot-strike detection algorithm F-VESPA using kinematic data stored in a .txt file. 

#### unit_GaitMonitor_tests
This test is implementing unit tests for the implemented Butterworth filter and the real-time kinematic-based foot-strike detection algorithm F-VESPA.

#### Vicon_GaitMonitor_tests
This test is implementing the real-time kinematic-based foot-strike detection algorithm F-VESPA using kinematic data streamed by Vicon Nexus. 


 ### include
This folder contains required libraries for the implementation of the ViconSDK process, responsible for receiving streaming kinematic data from the external software app "Vicon Nexus".

 ### util
This folder contains necessary libraries for the implementation of a shared memory between processes. 

## Publications
For more information regarding the F-VESPA algorithm, the reader is referred to the following publications:

**[Chrysostomos Karakasis and Panagiotis Arteamiadis, "F-VESPA: A Kinematic-based Algorithm for Real-time Heel-strike Detection During Walking." 2021 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS). IEEE, 2021.](https://doi.org/10.1109/IROS51168.2021.9636335)**

**[Chrysostomos Karakasis and Panagiotis Arteamiadis, "Real-time kinematic-based detection of foot-strike during walking." Journal of Biomechanics 129 (2021).](https://doi.org/10.1016/j.jbiomech.2021.110849)**


