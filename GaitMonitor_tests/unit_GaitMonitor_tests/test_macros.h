#include <iostream>
#include <iomanip>

// Define a custom testing macro for equality
#ifndef SUPPRESS_PASS
 #define SUPPRESS_PASS false
#endif

#define ASSERT_EQUAL(actual, expected) \
    do { \
        auto expected_val = (expected); \
        auto actual_val = (actual); \
        if (expected_val != actual_val) { \
            std::cerr << "\e[1;31m FAIL ASSERT_EQUAL: line " << __LINE__ << " --> " \
                      << "Expected: " << expected_val << ", Got: " << actual_val << "\e[0m" << std::endl; \
        } else { \
            if(!SUPPRESS_PASS) std::cout << "\e[1;32m PASS: line " << __LINE__ << "\e[0m" << std::endl; \
        } \
    } while (false)

// Define a custom testing macro for equality with a tolerance
#define ASSERT_EQUAL_TOL(actual, expected, tolerance) \
    do { \
        auto expected_val = (expected); \
        auto actual_val = (actual); \
        auto tol = (tolerance); \
        auto diff = (expected_val - actual_val); \
        if (diff > tol) { \
            std::cerr << std::setprecision(15) << "\e[1;31m FAIL ASSERT_EQUAL_TOL: line " << __LINE__ << " --> " \
                      << "Expected: " << expected_val << " Got: " << actual_val << " with tolerance of " << tol << " and diff of " << diff << "\e[0m" << std::endl; \
        } else { \
            if(!SUPPRESS_PASS) std::cout << "\e[1;32m PASS: line " << __LINE__ << "\e[0m" << std::endl; \
        } \
    } while (false)

// Define a custom testing macro for inequality
#define ASSERT_NOT_EQUAL(actual, expected) \
    do { \
        auto expected_val = (expected); \
        auto actual_val = (actual); \
        if (expected_val == actual_val) { \
            std::cerr << "\e[1;31m FAIL ASSERT_NOT_EQUAL: line " << __LINE__ << " --> " \
                      << "Expected: " << expected_val << ", Got: " << actual_val << std::endl; \
        } else { \
            if(!SUPPRESS_PASS) std::cout << "\e[1;32m PASS: line " << __LINE__ << "\e[0m" << std::endl; \
        } \
    } while (false)

// Define a custom testing macro for less than
#define ASSERT_LESS_THAN(exp1, exp2) \
    do { \
        auto exp_val1 = (exp1); \
        auto exp_val2 = (exp2); \
        if (!(exp_val1 < exp_val2)) { \
            std::cerr << std::setprecision(15) << "\e[1;31m FAIL ASSERT_LESS: line " << __LINE__ << " --> " \
                      << exp_val1 << " is not less than " << exp_val2 << std::endl; \
        } else { \
            if(!SUPPRESS_PASS) std::cout << "\e[1;32m PASS: line " << __LINE__ << "\e[0m" << std::endl; \
        } \
    } while (false)

// Define a custom testing macro for less than
#define ASSERT_GREATER_THAN(exp1, exp2) \
    do { \
        auto exp_val1 = (exp1); \
        auto exp_val2 = (exp2); \
        if (!(exp_val1 > exp_val2)) { \
            std::cerr << std::setprecision(15) << "\e[1;31m FAIL ASSERT_GREATER: line " << __LINE__ << " --> " \
                      << exp_val1 << " is not greater than " << exp_val2 << std::endl; \
        } else { \
            if(!SUPPRESS_PASS) std::cout << "\e[1;32m PASS: line " << __LINE__ << "\e[0m" << std::endl; \
        } \
    } while (false)

// Define a custom testing macro for throwing errors that takes an expression and the expected error
#define ASSERT_THROW(expression, expected_error) \
    do { \
        bool caught = false; \
        try { \
            expression; \
        } catch (const std::exception& e) { \
            caught = true; \
            if (std::string(e.what()) != std::string(expected_error)) { \
                std::cerr << "\e[1;31m FAIL ASSERT_THROW: line " << __LINE__ << " --> " \
                          << "Expected: " << expected_error << ", Got: " << e.what() << std::endl; \
            } else { \
            if(!SUPPRESS_PASS) std::cout << "\e[1;32m PASS: line " << __LINE__ << "\e[0m" << std::endl; \
            } \
        } \
        if (!caught) { \
            std::cerr << "\e[1;31m FAIL ASSERT_THROW: line " << __LINE__ << " --> " \
                      << "Expected: " << expected_error << ", Got: no exception" << std::endl; \
        } \
    } while (false)

