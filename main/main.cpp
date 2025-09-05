/**
 * @brief Entry point for the application.
 *
 * This file contains the main function which initializes and starts the application.
 *
 * @author Nils Termote
 * @date 2024-03-09
*/

#include <vector>
#include <Eigen/Dense>
#include <string>
#include <iostream>

//#include <fstream>
//#include <sstream>
//#include <filesystem>

#include "../include/CSV_parser.hpp"
#include "../include/EKF_predict.hpp"


void processFile(std::string filepath) {

    CSVP::CSVParser data{filepath};

    data.readCSV();
    std::vector<float> v = data.getVoltages();
    std::vector<float> c = data.getCurrents();

    float dt = 0.1f;          // 100 ms sampling            
    int temp = 25;            // room temperature
    int cap = 2500;           // typical 18650 cell, 2500 mAh
    float r0 = 0.015f;        // ohmic resistance
    float r1 = 0.005f;        // first RC resistance
    float c1 = 2000.0f;       // first RC capacitance
    float r2 = 0.003f;        // second RC resistance
    float c2 = 3000.0f;       // second RC capacitance

    Eigen::Matrix3f Q = Eigen::Matrix3f::Zero();
    Q(0,0 ) = 1e-4f;
    Q (1,1) =  1e-5f;
    Q (2,2) = 1e-5f;

    // Define initial state vector
    Eigen::Vector3f x;
    x << 0.5f, 0.0f, 0.0f;

    // Define intial covariance matrix
    Eigen::Matrix3f P;
    P.setIdentity();
    
    EKFPred::EKFPredict ekf{dt, temp, cap, r0, r1, c1, r2, c2};

    // Loop will start to iterate through CSV
    for (int i = 0; i < c.size(); i ++)  {
        float cur = c[i];
        float vol = v[i];


        if (i == 0) {
            
        } else {
            x = ekf.predictState(x, cur); // calculate prior state
            P = ekf.predictCovariance(P, Q); //calculate prior covariance
        };

        std::cout << x << std::endl;
        std::cout << P << std::endl;


        // calculate posterior
        // Extract SOC from posterior state vector and save to file
    
    };
    std::cout << "succeeded" << std::endl;

};

int main (int argc, char **argv) {
    
    processFile(argv[1]);

    return 0;
}
    
// /home/ntermote/Documents/ExR_files/estimator/data/ccd_cycle_1a.csv"



