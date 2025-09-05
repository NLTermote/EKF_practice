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
EKFPredict::EKFPredict(float dt, int temp, int cap, float r0, float r1, float c1, float r2, float c2) : 
    delta_t(dt), temperature(temp), capacity(cap), R0(r0), R1(r1), C1(c1), R2(r2), C2(c2) {}

// Method for obtaining A matrix
Eigen::Matrix3f EKFPredict::getMatrixA() {
    float A1 = std::exp((-delta_t/(R1*C1)));
    float A2 = std::exp((-delta_t/(R2*C2)));

    Eigen::Matrix3f A;
    A << 1.0f, 0.0f, 0.0f, 0.0f, A1, 0.0f, 0.0f, 0.0f, A2;

    return A;
}

// Method for obtaining matrix B
Eigen::Vector3f EKFPredict::getMatrixB() {
    float B1 = -delta_t/capacity;
    float B2 = R1 * (1 - std::exp(-delta_t/(R1*C1)));
    float B3 = R2 * (1 - std::exp(-delta_t/(R2*C2)));

    Eigen::Vector3f B;
    B << B1, B2, B3;

    return B;

}

// Method for predicting state
Eigen::Vector3f EKFPredict::predictState(Eigen::Vector3f x, float cur) {
    Eigen::Matrix3f A = getMatrixA();
    Eigen::Vector3f B = getMatrixB();
    
    Eigen::Vector3f x_prior = A * x + B * cur; 

    return x_prior;
};

// Method for predicting covariance 
Eigen::Matrix3f EKFPredict::predictCovariance(Eigen::Matrix3f P, Eigen::Matrix3f Q) {
    
    Eigen::Matrix3f A = getMatrixA();
    Eigen::Matrix3f P_prior = A * P * A.transpose() + Q;

    return P_prior;
};

};