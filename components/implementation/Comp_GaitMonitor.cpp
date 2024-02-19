// Definition and analysis of the member functions included in the GaitMonitor classes

#include "components/Comp_GaitMonitor.h"
#include <iostream>
#include <chrono>

// Define constants
#ifndef M_PI 
#define M_PI 3.14159
#endif

using namespace std; 

// Constructor for Butterworth Filter class invoked automatically when a "ButterworthFilter" object is created
ButterworthFilter::ButterworthFilter(double cutoffFreq, double sampleFreq) {
    this->fc = cutoffFreq;          // set the cutoff frequency
    this->Fs = sampleFreq;          // set the sampling frequency
    this->init();                   // call the initializing function
}

// Public member function of ButterworthFilter class responsible for implementing the filter
// Input: new sample of the signal to be filtered 
// Output: filtered sample of the signal
double ButterworthFilter::filter(double input) {
    // Linear difference equation of the discrete-time second order Butterworth (digital) filter
    double output = (b1 * input + b2 * x_n_minus_1 + b3 * x_n_minus_2 - a2 * y_n_minus_1 - a3 * y_n_minus_2) / a1;

    // Update the state variables (previous inputs and outputs)
    x_n_minus_2 = x_n_minus_1;
    x_n_minus_1 = input;
    y_n_minus_2 = y_n_minus_1;
    y_n_minus_1 = output;
    return output;
}

// Initialization function of ButterworthFilter class
void ButterworthFilter::init() {
    omega_c = 2 * M_PI * fc;                    // Calculate the cutoff frequency in rad/s
    T = 1 / Fs;                                 // Calculate the sampling period
    // Calculate the filter coefficients
    b1 = pow(omega_c * T, 2);
    b2 = 2 * b1;
    b3 = b1;
    a1 = 4 + 2 * sqrt(2) * omega_c * T + b1;
    a2 = -8 + 2 * b1;
    a3 = 4 - 2 * sqrt(2) * omega_c * T + b1;
}

//---------------------------------------------------------------------------------
// Foot Strike Detection Functions

// Constructor for FootStrikeDetector class invoked automatically when a "FootStrikeDetector" object is created
FootStrikeDetector::FootStrikeDetector() {
    // Initialize the variables of interest
    this->init();
}

// Public member function of FootStrikeDetector class responsible for implementing the F-VESPA algorithm
// Inputs: Vicon Nexus frame number, new filtered sample of the vertical and sagittal position of the heel marker (left or right)
bool FootStrikeDetector::FVESPA(int frame,double heel_vert_new_f, double heel_sag_new_f){

        // Calculate velocity of the heel marker in the vertical and sagittal directions
        vel_z = heel_vert_new_f - heel_vert_filt_one_sample_ago;
        vel_s = heel_sag_new_f - heel_sag_filt_one_sample_ago;

        // Set flag showing whether a foot-strike took place to false by default (will be set to true if a foot-strike is detected)
        foot_strike_flag = false;

        // Condition for detecting a foot-strike
        // Necessary for Vicon F.S. and extra check that foot-strikes are not detected in swing phase
        // if (vel_z>=-0.01 && vel_prev_1<=0 && vel_prev_2<=0 && vel_prev_3<=0 && search_flag == 0 && vel_s < 6.01 && heel_vert_new_f < 500){ //for prosthesis
        if (vel_z>=0 && vel_prev_1<=0 && vel_prev_2<=0 && vel_prev_3<=0 && search_flag == true && vel_s<=0 && heel_vert_new_f<500){
            
            // Update the minimum value of the vertical position of the heel marker
            min_heel = heel_vert_filt_one_sample_ago;

            // Register the frame number of the foot-strike
            last_hs_frame = frame-1;

            // Ensure the time stemp of the previous foot-strike is updated (necessary for fail-safe mechanism of the gait monitor)
            time_stamp_hs_prev = time_stamp_hs;
            // Calculate the time stamp of the foot-strike
            auto current_time = chrono::high_resolution_clock::now();
            time_stamp_hs = chrono::duration_cast<chrono::microseconds>(current_time.time_since_epoch()).count() / 1e6;
            // Calculate the duration of the last gait cycle in seconds
            new_duration = time_stamp_hs - time_stamp_hs_prev; 

            // Update the rolling sum and gait_cycle_duration_vector elements
            temp_sum = temp_sum - gait_cycle_duration_vector[0] + new_duration;

            // Use std::rotate to update gait_cycle_duration_vector
            rotate(gait_cycle_duration_vector.begin(), gait_cycle_duration_vector.begin() + 1, gait_cycle_duration_vector.end());
            gait_cycle_duration_vector[4] = new_duration;

            // Calculate the average duration of the last five gait cycles
            gait_cycle_duration = ( temp_sum )/5; 
            // Update the time stamp of the previous foot-strike
            time_stamp_hs_prev = time_stamp_hs;

            // Increase the counter of the gait cycles
            gait_cycle = gait_cycle+1; 

            // // Print the frame number of the foot-strike
            // cout << "Heel-strike at Frame: "<< frame-1 << endl;
            
            // Set the flag showing whether a foot-strike took place to true
            foot_strike_flag = true;

            // Set the search flag to false to avoid detecting a new foot-strike in the same gait cycle
            search_flag = false;
        }
        else if (frame>2 && vel_z<0 && vel_prev_1<=0 && vel_prev_2>=0 && vel_prev_3>=0 && ((heel_vert_filt_two_samples_ago-min_heel)>100)){
            // Condition for detecting the frame where the heel marker reaches its maximum vertical position
            
            // Enable the search for a new foot-strike (this avoid detecting a new foot-strike during swing phase)
            search_flag = true;
        }

        // Update the previous velocity values
        vel_prev_3 = vel_prev_2;
        vel_prev_2 = vel_prev_1;
        vel_prev_1 = vel_z;

        // Update the previous filtered position values
        heel_vert_filt_two_samples_ago = heel_vert_filt_one_sample_ago;
        heel_vert_filt_one_sample_ago = heel_vert_new_f;
        heel_sag_filt_one_sample_ago = heel_sag_new_f;

        // Return the flag showing whether a foot-strike took place
        return foot_strike_flag;
}

// Initialization function of FootStrikeDetector class
void FootStrikeDetector::init() {
    min_heel = -1000;                           // initialize the minimum value of the vertical position of the heel marker to an non-realistic negative value
    search_flag = false;                        // initialize the search flag to false
    vel_prev_1 = 0;                             // initialize the previous velocity values to zero
    vel_prev_2 = 0;                             // initialize the previous velocity values to zero
    vel_prev_3 = 0;                             // initialize the previous velocity values to zero
    heel_vert_filt_one_sample_ago = 0;          // initialize the filtered position of the heel marker in the vertical direction one sample ago to zero
    heel_vert_filt_two_samples_ago = 0;         // initialize the filtered position of the heel marker in the vertical direction two samples ago to zero
    heel_sag_filt_one_sample_ago = 0;           // initialize the filtered position of the heel marker in the sagittal direction one sample ago to zero
    temp_sum = 0;                               // initialize the rolling sum to zero
    new_duration = 0;                           // initialize the duration of the last gait cycle to zero
    last_hs_frame = 0;                          // initialize the frame number of the previous foot-strike to zero
    time_stamp_hs_prev = 0;                     // initialize the time stamp of the previous foot-strike to zero
    gait_cycle = 1;                             // initialize the counter of the gait cycles to 1
    gait_cycle_duration_vector = {0,0,0,0,0};   // initialize the vector storing the duration of the last five gait cycles with zeros
}