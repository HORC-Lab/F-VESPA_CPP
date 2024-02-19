// Gait Monitor interface

#ifndef COMP_GAIT_MONITOR_H
#define COMP_GAIT_MONITOR_H

#include <vector>

// Define a class implementing a second order Butterworth filter
class ButterworthFilter {
public:
    ButterworthFilter(double cutoffFreq, double sampleFreq);
    double filter(double input);

private:
    double fc;                              // [Hz] Cutoff frequency
    double Fs;                              // [Hz] Sampling frequency
    double omega_c;                         // [rad/s] Cutoff frequency
    double T;                               // [s] Sampling period
    double a1, a2, a3, b1, b2, b3;          // Filter coefficients

    // State variables for the filter
    double x_n_minus_1, x_n_minus_2;        // Previous inputs
    double y_n_minus_1, y_n_minus_2;        // Previous outputs

    void init();
};


// Define a class implementing a foot-strike detector algorithm
class FootStrikeDetector {
public:
    FootStrikeDetector();

    // define protorype of public member fuction responsible for implementing the F-VESPA algorithm
    bool FVESPA(int frame,double heel_vert_new_f, double heel_sag_new_f);

    // Define the variables of interest that will be propagated to the shared memory
    int last_hs_frame, gait_cycle;
    double gait_cycle_duration,time_stamp_hs;

private:
	int search_flag;
    bool foot_strike_flag;
	double min_heel,vel_prev_1,vel_prev_2,vel_prev_3;
	double heel_vert_new_f,heel_sag_new_f,vel_z,vel_s;
	double heel_vert_filt_one_sample_ago,heel_vert_filt_two_samples_ago;
	double heel_sag_filt_one_sample_ago;  
	double temp_sum,new_duration;
	double time_stamp_hs_prev;
    std::vector<double> gait_cycle_duration_vector;
    void init();
};

#endif