// Test Gait Monitor Process

// Here, pre-recorded Vicon data are read from a .txt file and loaded to a shared memory.
// Then, a GaitMonitor process is loading the kinematic data from the shared memory and 
// foot-strike events are detected using the real-time F-VESPA algorithm for the left foot. 
// The newly calculated foot-strike frame number is loaded from the shared memory, 
// and it is compared to the results of an offline implementation of F-VESPA in MATLAB
// to check the accuracy of the real-time F-VESPA algorithm.

#include "util/MemManager.h" 
#include <fstream>
#include <thread>

using namespace std; 

int main() {
    // Set up connection to the shared memory
    MemManager SharedMem(L"MySharedMemory");
    SharedMem.Create();
    
    // Load input file that contains the Vicon data
    ifstream infile("test_input_files/testing_vicon_input_healthy_subj_vst2.txt");

    // Check if the file is open
    if (!infile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    // Variables to save the frame number of the last heel-strike event detected by the offline F-VESPA algorithm
    int offline_fvespa_fs,last_realtime_fvespa_fs;
    last_realtime_fvespa_fs = 0;
    SharedMem.data->experiment_state = ExpStates::NOT_STARTED; // Initialize the experiment state to NOT_STARTED
    // Variable to store the user input
    float input;
	// 1: Experiment Running
	// 2: End Experiment
	cout << "Enter 1 to start experiment and 2 to end experiment: ";
	cin >> input;
    if (input == 1){
		SharedMem.data->experiment_state = ExpStates::RUNNING;
        cout << "Experiment Started" << endl;
	}
	else if (input == 2){
		SharedMem.data->experiment_state = ExpStates::END;
	}
	else{
		cout << "Invalid Input" << endl;
	}

    // Start an infinite loop
    while(true) {

        switch (SharedMem.data->experiment_state) { //Check whether user has selected experiment mode yet or not
            case ExpStates::NOT_STARTED:
                //Twiddle thumbs
                break;

            case ExpStates::RUNNING:
				// Read a line from the input file and separate columns based on gaps and store them in 3 variables
				infile >> SharedMem.data->frame >> SharedMem.data->LHEEy >> SharedMem.data->LHEEz >> offline_fvespa_fs;

                // Check whether a new foot-strike event has been detected by the real-time F-VESPA algorithm and 
                // compare with the offline F-VESPA algorithm implemented in MATLAB
                if (SharedMem.data->left_last_hs_frame != last_realtime_fvespa_fs){
                    // New foot-strike event detected by the offline F-VESPA algorithm
                    cout << "Real-time F-VESPA FS: " << SharedMem.data->left_last_hs_frame << " Offline F-VESPA FS: " << offline_fvespa_fs  << endl;
                    // Update the frame number of the last heel-strike event detected by the offline F-VESPA algorithm
                    last_realtime_fvespa_fs = SharedMem.data->left_last_hs_frame;
                }

                // Sleep for 0.01 seconds (10 milliseconds) to similate the 100 Hz sampling rate of Vicon
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                if (infile.eof()) {
                    // End of file is reached
                    SharedMem.data->experiment_state = ExpStates::END;
                }

                break; 

            case ExpStates::END:
                cout << "Terminating Loop, Ending Experiment";
				infile.close();
                SharedMem.Disconnect();
                return 0;

            default:
                break;
        }
    }
}