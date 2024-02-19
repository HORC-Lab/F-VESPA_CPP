#pragma once // Ensure inclusion only once

#include <iostream>

/*  This is the struct which defines the size and layout for our memory mapped file (shared memory)
*   Think of it a bit as being a bit like a template for our shared memory. It defines what our database looks like
*   and how it is layed out. Every process accessing the memory must include this file in order to use it 
*   when connecting to the shared memory. We can add onto this shared data as needed, creating variables that
*   all our processes can access.
*   
*   NOTE: This SharedData struct lives in shared_data.h so that it can be included in other files. This means we only
*   have one version of the shared data struct to maintain, making things smoother.
*/

// Enum defining possible experiment states
enum class ExpStates {
    NOT_STARTED = 0,
    RUNNING,
    END
};

// overload on  << to print expstates
inline std::ostream& operator<<(std::ostream& os, const ExpStates& state) {
    switch (state) {
        case ExpStates::NOT_STARTED:
            os << "NOT_STARTED";
            break;
        case ExpStates::RUNNING:
            os << "RUNNING";
            break;
        case ExpStates::END:
            os << "END";
            break;
        default:
            os.setstate(std::ios_base::failbit);
            break;
    }
    return os;
}



struct SharedMemStruct {
    int value1;
    int value2;
    ExpStates experiment_state;
    bool VSTcontrol_ready = false;
    bool ForcematHandler_ready = false;
    bool UserInterface_ready = false;
    bool EncoderHandler_ready = false;
    bool DAQ_state = false;
    float vsm_left_dStiffness_kNpm;     // Desired stiffness of left vsm [kN/m]
    float vsm_right_dStiffness_kNpm;    // Desired stiffness of right vsm [kN/m]
    int vsm_left_aPos_cnts;             // Actual position of left vsm [counts]
    int vsm_right_aPos_cnts;            // Actual position of right vsm [counts]
    float belt_left_dVel_mps;           // Desired linear velocity of left belt [m/s]
    float belt_right_dVel_mps;          // Desired linear velocity of right belt [m/s]
    float belt_left_aVel_rpm;           // Actual angular velocity of left belt motor [rpm]
    float belt_right_aVel_rpm;          // Actual angular velocity of right belt motor [rpm]
    float vsm_left_RMS;                 // RMS of left vsm motor
    float vsm_right_RMS;                // RMS of right vsm motor
    float belt_left_RMS;                // RMS of left belt motor
    float belt_right_RMS;               // RMS of right belt motor
    double RHEEx;                       // Right heel marker
    double RHEEy;                       // Right heel marker
    double RHEEz;                       // Right heel marker
    double LHEEx;                       // Left heel marker
    double LHEEy;                       // Left heel marker
    double LHEEz;                       // Left heel marker
    double RTOEx;                       // Right TOE marker
    double RTOEy;                       // Right TOE marker
    double RTOEz;                       // Right TOE marker
    double LTOEx;                       // Left TOE marker
    double LTOEy;                       // Left TOE marker
    double LTOEz;                       // Left TOE marker
    int frame;                          // Frame number      
    int left_gc;                        // Left Gait cycle number
    double left_gc_pct;                 // Left Gait cycle percentage
    int left_last_hs_frame;             // Frame number of last left foot-strike
    double left_gc_dur;                 // Average duration of left gait cycle in seconds
    double left_time_stamp_hs;          // Time stamp of left foot-strike
    int right_gc;                       // Left Gait cycle number
    double right_gc_pct;                // Left Gait cycle percentage
    int right_last_hs_frame;            // Frame number of last left foot-strike
    double right_gc_dur;                // Average duration of left gait cycle in seconds
    double right_time_stamp_hs;         // Time stamp of left foot-strike
     // Other variables (can be different types!) added here as needed
};