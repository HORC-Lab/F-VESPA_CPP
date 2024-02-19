// Test Gait Monitor Process

// Here, the classes defined in the Comp_GaitMonitor.cpp file are used to process the Vicon data and detect foot-strike events.
// The Vicon data is read from the shared memory and the foot-strike events are detected using the real-time F-VESPA algorithm for both the left and the right foot. 
// The corresponding gait cycle number, last foot-strike frame number, gait cycle duration and time stamp of the foot-strike events are written to the shared memory.
// For more information on the F-VESPA algorithm, please refer to the following papers by Karakasis and Artemiadis: 
// https://doi.org/10.1109/IROS51168.2021.9636335
// https://doi.org/10.1016/j.jbiomech.2021.110849

#include "components/Comp_GaitMonitor.h"
#include "util/MemManager.h"
#include <chrono>

// Define constants
#ifndef M_PI 
#define M_PI 3.14159
#endif
using namespace std; 

int main() {

	// Set up connection to the shared memory
    MemManager SharedMem(L"Vicon_SharedMemory");
    SharedMem.Connect();

	// Print out message to indicate that the connection to the shared memory has been established
	cout << "Connected to Shared Memory" << endl;

	// Declare two ButterworthFilter objects of specicied cutoff frequency and sampling frequency
    double cutoffFrequency = 20; 		// Hz
    double samplingFrequency = 100; 	// Hz
    ButterworthFilter filter_lhee_y(cutoffFrequency, samplingFrequency);
    ButterworthFilter filter_lhee_z(cutoffFrequency, samplingFrequency);
    ButterworthFilter filter_rhee_y(cutoffFrequency, samplingFrequency);
    ButterworthFilter filter_rhee_z(cutoffFrequency, samplingFrequency);

	// Declare a FootStrikeDetector object to detect foot-strike events for both feet
    FootStrikeDetector left_foot;
    FootStrikeDetector right_foot;

	int iter_count;
    double current_time_sec;
    // Fail-safe mechanism variables
    int left_fail_safe_hs_frame, right_fail_safe_hs_frame;
    double left_fail_safe_ts, right_fail_safe_ts;
    int fail_safe_flag = -1;
	//----------- Initialization -----------------//
	iter_count = 1;	// Initialize the local frame number to 1
    SharedMem.data->experiment_state = ExpStates::RUNNING;
    auto current_time = chrono::high_resolution_clock::now();
    current_time_sec = chrono::duration_cast<chrono::microseconds>(current_time.time_since_epoch()).count() / 1e6;
    SharedMem.data->left_time_stamp_hs = current_time_sec;
    SharedMem.data->left_gc_dur = 1;
    SharedMem.data->right_time_stamp_hs = current_time_sec;
    SharedMem.data->right_gc_dur = 1;
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
					// (4) Check whether a new LEFT foot-strike event has been detected or not for the new frame
                    if (left_foot.FVESPA(SharedMem.data->frame,filter_lhee_z.filter(SharedMem.data->LHEEz),filter_lhee_y.filter(SharedMem.data->LHEEy))){
						//New heel-strike detected - update shared memory
						SharedMem.data->left_gc = left_foot.gait_cycle;
						SharedMem.data->left_last_hs_frame = left_foot.last_hs_frame;
						SharedMem.data->left_gc_dur = left_foot.gait_cycle_duration;
						SharedMem.data->left_time_stamp_hs = left_foot.time_stamp_hs;
                        cout << "Left Foot Strike: " << SharedMem.data->left_last_hs_frame << " LGC:" << SharedMem.data->left_gc << " RGC:" << SharedMem.data->right_gc << " LGCP: " << SharedMem.data->left_gc_pct << " RGCP: " << SharedMem.data->right_gc_pct  <<  endl;
                        
                        // Fail-safe mechanism to handle missed foot-strike events during gait cycles
                        if(fail_safe_flag == -1 || fail_safe_flag == 1){// fail_safe_flag = -1: only in the first run, fail_safe_flag = 1 means that a right foot-strike was detected after the last left foot-strike
                            fail_safe_flag = 0;                         // fail_safe_flag = 0 means that a left foot-strike was detected after the last right foot-strike
                        }
                        else if(fail_safe_flag == 0){ // fail_safe_flag = 0: here it means that no right foot-strike was detected after the last left foot-strike
                            // If two consecutive left foot-strikes are detected without a right foot-strike in between, then the right foot-strike is assumed to have been missed
                            cout << "!!! Right Foot Strike Missed at Vicon Frame: " << SharedMem.data->frame  << endl;
                            // Update the public variables of the right FootStrikeDetector object, as if a right foot-strike was detected when the gait cycle percentage exceeded 1
                            right_foot.gait_cycle = right_foot.gait_cycle + 1;
                            // Assume that the missed foot-strike occured at the frame number and time stamp stored in the fail-safe variables
                            right_foot.last_hs_frame = right_fail_safe_hs_frame;
                            right_foot.time_stamp_hs = right_fail_safe_ts;
                            // gait cycle duration is not affected and is assumed to be the same as the previous gait cycle
                            // Update shared memory
                            SharedMem.data->right_gc = right_foot.gait_cycle;
                            SharedMem.data->right_last_hs_frame = right_foot.last_hs_frame;
                            SharedMem.data->right_time_stamp_hs = right_foot.time_stamp_hs;
                        }

                    }
					// (5) Check whether a new RIGHT foot-strike event has been detected or not for the new frame
					if (right_foot.FVESPA(SharedMem.data->frame,filter_rhee_z.filter(SharedMem.data->RHEEz),filter_rhee_y.filter(SharedMem.data->RHEEy))){
						//New heel-strike detected - update shared memory
						SharedMem.data->right_gc = right_foot.gait_cycle;
						SharedMem.data->right_last_hs_frame = right_foot.last_hs_frame;
						SharedMem.data->right_gc_dur = right_foot.gait_cycle_duration;
						SharedMem.data->right_time_stamp_hs = right_foot.time_stamp_hs;
                        cout << "Right Foot Strike: " << SharedMem.data->right_last_hs_frame << " LGC:" << SharedMem.data->left_gc << " RGC:" << SharedMem.data->right_gc << " LGCP: " << SharedMem.data->left_gc_pct << " RGCP: " << SharedMem.data->right_gc_pct  <<  endl;
					
                    // Fail-safe mechanism to handle missed foot-strike events during gait cycles
                        if(fail_safe_flag == -1 || fail_safe_flag == 0){// fail_safe_flag = -1: only in the first run, fail_safe_flag = 0 means that a left foot-strike was detected after the last right foot-strike
                            fail_safe_flag = 1;                         // fail_safe_flag = 1 means that a right foot-strike was detected after the last left foot-strike
                        }
                        else if(fail_safe_flag == 1){// fail_safe_flag = 1: here it means that no left foot-strike was detected after the last right foot-strike
                            // If two consecutive right foot-strikes are detected without a left foot-strike in between, then the left foot-strike is assumed to have been missed
                            cout << "!!! Left Foot Strike Missed at Vicon Frame: " << SharedMem.data->frame << endl;
                            // Update the public variables of the left FootStrikeDetector object, as if a left foot-strike was detected when the gait cycle percentage exceeded 1
                            left_foot.gait_cycle = left_foot.gait_cycle + 1;
                            // Assume that the missed foot-strike occured at the frame number and time stamp stored in the fail-safe variables
                            left_foot.last_hs_frame = left_fail_safe_hs_frame;
                            left_foot.time_stamp_hs = left_fail_safe_ts;
                            // gait cycle duration is not affected and is assumed to be the same as the previous gait cycle
                            // Update shared memory
                            SharedMem.data->left_gc = left_foot.gait_cycle;
                            SharedMem.data->left_last_hs_frame = left_foot.last_hs_frame;
                            SharedMem.data->left_time_stamp_hs = left_foot.time_stamp_hs;
                        }
                    }
				}

                // Update the left and right gait cycle percentages
                // Calculate the current time stamp and convert it to seconds
                current_time = chrono::high_resolution_clock::now();
                current_time_sec = chrono::duration_cast<chrono::microseconds>(current_time.time_since_epoch()).count() / 1e6;
                // Update left gait cycle percentage, calculated as the time passed since the last left foot-strike in seconds and divided over average gait cycle duration
                SharedMem.data->left_gc_pct = (current_time_sec - SharedMem.data->left_time_stamp_hs)/SharedMem.data->left_gc_dur;
                // Update right gait cycle percentage, calculated as the time passed since the last right foot-strike in seconds and divided over average gait cycle duration
                SharedMem.data->right_gc_pct = (current_time_sec - SharedMem.data->right_time_stamp_hs)/SharedMem.data->right_gc_dur;

                // The code below is used for the fail-safe mechanism
                // Whenever the left gait cycle percentage is greater than 1, store the frame number and time stamp for backup
                if(SharedMem.data->left_gc_pct > 1){
                    left_fail_safe_hs_frame = SharedMem.data->frame;
                    left_fail_safe_ts = current_time_sec;
                }
                // Whenever the right gait cycle percentage is greater than 1, store the frame number and time stamp for backup
                if(SharedMem.data->right_gc_pct > 1){
                    right_fail_safe_hs_frame = SharedMem.data->frame;
                    right_fail_safe_ts = current_time_sec;
                }
                // The code above is used for the fail-safe mechanism

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