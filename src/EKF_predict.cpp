/**
 * @brief EKF predict step class
 *
 * This file contains the class for the predict step of the EKF
 *
 * @author Nils Termote
 * @date 2024-04-09
*/

#include <cmath>
#include <Eigen/Dense>

#include "../include/EKF_predict.hpp"

namespace EKFPred {

// Predict class constructor
EKFPredict::EKFPredict(float dt, float cur, float vol, int temp, int cap, float r0, float r1, float c1, float r2, float c2) : 
    delta_t(dt), current_load(cur), voltage_load(vol), temperature(temp), capacity(cap), R0(r0), R1(r1), C1(c1), R2(r2), C2(c2) {}

// Class method for obtaining A matrix
Eigen::Matrix3f getMatrixA(float delta_t, float R1, float C1, float R2, float C2) {
    float A1 = std::exp((-delta_t/(R1*C1)));
    float A2 = std::exp((-delta_t/(R1*C1)));

    Eigen::Matrix3f A;
    A << 1.0f, 0.0f, 0.0f, 0.0f, A1, 0.0f, 0.0f, 0.0f, A2;

    return A;
}

}