// Test Gait Monitor Process

// Here, the classes defined in the Comp_GaitMonitor.cpp file are used to process the Vicon data and detect foot-strike events.
// The Vicon data is read from the shared memory and the foot-strike events are detected using the real-time F-VESPA algorithm for the left foot. 
// The corresponding gait cycle number, last foot-strike frame number, gait cycle duration and time stamp of the heel-strike event are written to the shared memory.
// For more information on the F-VESPA algorithm, please refer to the following papers by Karakasis and Artemiadis: 
// https://doi.org/10.1109/IROS51168.2021.9636335
// https://doi.org/10.1016/j.jbiomech.2021.110849

#include "components/Comp_GaitMonitor.h"
#include "util/MemManager.h"

// Define constants
#ifndef M_PI 
#define M_PI 3.14159
#endif
using namespace std; 

int main() {

	// Set up connection to the shared memory
    MemManager SharedMem(L"MySharedMemory");
    SharedMem.Connect();

	// Print out message to indicate that the connection to the shared memory has been established
	cout << "Connected to Shared Memory" << endl;

	// Declare two ButterworthFilter objects of specicied cutoff frequency and sampling frequency
    double cutoffFrequency = 20; 		// Hz
    double samplingFrequency = 100; 	// Hz
    ButterworthFilter filter_lhee_y(cutoffFrequency, samplingFrequency);
    ButterworthFilter filter_lhee_z(cutoffFrequency, samplingFrequency);

	// Declare a FootStrikeDetector object to detect foot-strike events
    FootStrikeDetector left_foot;

	int iter_count;

	//----------- Initialization -----------------//
	iter_count = 1;	// Initialize the local frame number to 1

    // Start an infinite loop
    while(true) {

        switch (SharedMem.data->experiment_state) { //Check whether user has selected experiment mode yet or not
            case ExpStates::NOT_STARTED:
                //Twiddle thumbs
				cout << "Waiting for experiment to start" << endl;
                break;

            case ExpStates::RUNNING:	// Experiment is running

                // Run only when a new frame has been received from Vicon
                if (SharedMem.data->frame != iter_count){
                // Read all the variables and write to shared memory

					iter_count = SharedMem.data->frame; //Update the local frame number

					// (1) Load the new raw samples of the heel marker position (y and z) from the shared memory
					// (2) Filter the new samples using the "filter" method of the "Butterworthfilter" class
					// (3) Use the filtered sampled as inputs for the F-VESPA algorithm to detect foot-strike events
					// (4) Check whether a new foot-strike event has been detected or not for the new frame
					if (left_foot.FVESPA(SharedMem.data->frame,filter_lhee_z.filter(SharedMem.data->LHEEz),filter_lhee_y.filter(SharedMem.data->LHEEy))){
						//New heel-strike detected - update shared memory
						SharedMem.data->left_gc = left_foot.gait_cycle;
						SharedMem.data->left_last_hs_frame = left_foot.last_hs_frame;
						SharedMem.data->left_gc_dur = left_foot.gait_cycle_duration;
						SharedMem.data->left_time_stamp_hs = left_foot.time_stamp_hs;
					}
				}

                break;
            
            case ExpStates::END:
                cout << "Terminating Loop, Ending Experiment";
                SharedMem.Disconnect();
                return 0;

            default:
                break;
        }
    }
}