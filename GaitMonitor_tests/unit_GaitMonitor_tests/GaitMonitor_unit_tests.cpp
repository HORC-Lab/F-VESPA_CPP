// GaitMonitor_unit_tests.cpp
// Creator: Chrysostomos Karakasis

// Description: Write unit tests for all methods for the classes in Comp_GaitMonitor.h
// compare with values extracted by Matlab. 

// Last edit: 2/18/2024

// Future work: Switch to google test framework

#include "GaitMonitor_tests/unit_GaitMonitor_tests/test_macros.h"
#include "components/Comp_GaitMonitor.h"

using namespace std; 

// Define pi if not already defined
#ifndef M_PI 
#define M_PI 3.14159
#endif

int main(int argc, char **argv) {

    // Declare a ButterworthFilter object with specific cutoff and sampling frequencies
    double cutoffFrequency = 20; // Hz
    double samplingFrequency = 100; // Hz
    ButterworthFilter filter_dummy(cutoffFrequency, samplingFrequency);

    std::cout << std::endl;
    std::cout << "===== Butterworth Filter tests =====" << std::endl;

    // Notice how initially the filtered values are quite different from the raw values, 
    // while they get closer to the raw values at the latter samples.
    ASSERT_EQUAL_TOL(filter_dummy.filter(690.129028), 119.3206,0.001);
    ASSERT_EQUAL_TOL(filter_dummy.filter(697.071411), 422.4152,0.001);
    ASSERT_EQUAL_TOL(filter_dummy.filter(702.212158), 679.2459,0.001);
    ASSERT_EQUAL_TOL(filter_dummy.filter(705.408997), 751.7308,0.001);
    ASSERT_EQUAL_TOL(filter_dummy.filter(706.516418), 735.4036,0.001);
    ASSERT_EQUAL_TOL(filter_dummy.filter(705.519226), 711.4421,0.001);
    ASSERT_EQUAL_TOL(filter_dummy.filter(702.431274), 701.6724,0.001);


	// Declare a FootStrikeDetector object to detect foot-strike events
    FootStrikeDetector left_foot;

    std::cout << std::endl;
    std::cout << "===== Foot-strike Detection tests =====" << std::endl;
    // The FVESPA method is boolean, so first we will test the boolean values
    // and then the corresponding values of the public variables

    ASSERT_EQUAL(left_foot.FVESPA(1,81.9513,39.9065), 0); // false
    ASSERT_EQUAL(left_foot.gait_cycle, 1);
    ASSERT_EQUAL(left_foot.last_hs_frame, 0);
    ASSERT_EQUAL_TOL(left_foot.gait_cycle_duration, 0,0.001);
    ASSERT_EQUAL_TOL(left_foot.time_stamp_hs, 0,0.001);

    ASSERT_EQUAL(left_foot.FVESPA(2,289.3255,140.2264), 0);
    ASSERT_EQUAL(left_foot.gait_cycle, 1);
    ASSERT_EQUAL(left_foot.last_hs_frame, 0);
    ASSERT_EQUAL_TOL(left_foot.gait_cycle_duration, 0,0.001);
    ASSERT_EQUAL_TOL(left_foot.time_stamp_hs, 0,0.001);

    ASSERT_EQUAL(left_foot.FVESPA(4,509.3614,240.8251), 0);
    ASSERT_EQUAL(left_foot.gait_cycle, 1);
    ASSERT_EQUAL(left_foot.last_hs_frame, 0);
    ASSERT_EQUAL_TOL(left_foot.gait_cycle_duration, 0,0.001);
    ASSERT_EQUAL_TOL(left_foot.time_stamp_hs, 0,0.001);

    ASSERT_EQUAL(left_foot.FVESPA(5,495.4431,229.5268), 0);
    ASSERT_EQUAL(left_foot.gait_cycle, 1);
    ASSERT_EQUAL(left_foot.last_hs_frame, 0);
    ASSERT_EQUAL_TOL(left_foot.gait_cycle_duration, 0,0.001);
    ASSERT_EQUAL_TOL(left_foot.time_stamp_hs, 0,0.001);

    ASSERT_EQUAL(left_foot.FVESPA(6,477.9329,216.5277), 0);
    ASSERT_EQUAL(left_foot.gait_cycle, 1);
    ASSERT_EQUAL(left_foot.last_hs_frame, 0);
    ASSERT_EQUAL_TOL(left_foot.gait_cycle_duration, 0,0.001);
    ASSERT_EQUAL_TOL(left_foot.time_stamp_hs, 0,0.001);

    ASSERT_EQUAL(left_foot.FVESPA(7,471.8558,209.2244), 0);
    ASSERT_EQUAL(left_foot.gait_cycle, 1);
    ASSERT_EQUAL(left_foot.last_hs_frame, 0);
    ASSERT_EQUAL_TOL(left_foot.gait_cycle_duration, 0,0.001);
    ASSERT_EQUAL_TOL(left_foot.time_stamp_hs, 0,0.001);

    // Foot-strike should be detected in the next frame
    // Notice how all public variables are updated
    ASSERT_EQUAL(left_foot.FVESPA(8,472.6185,205.4473), 1); // true
    ASSERT_EQUAL(left_foot.gait_cycle, 2);
    ASSERT_EQUAL(left_foot.last_hs_frame, 7);
    ASSERT_GREATER_THAN(left_foot.gait_cycle_duration, 0); // actual value depends on computer speed, hence a specific value is not used
    ASSERT_GREATER_THAN(left_foot.time_stamp_hs, 0);  // actual value depends on computer speed, hence a specific value is not used

    return 0;
}

