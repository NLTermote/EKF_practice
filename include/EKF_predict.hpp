/**
 * @brief EKF predict step class header file
 *
 * This file contains the header for the class for the predict step of the EKF
 *
 * @author Nils Termote
 * @date 2024-04-09
*/

#pragma once

#include <cmath>
#include <Eigen/Dense>

namespace EKFPred {

    class EKFPredict {
        public:

        /*
        Goal is to make a class that takes the following values:
            from CSV
                delta t
                current_load
                voltage_load

            from predefined array
                temperature T
                capacity Q
                R0
                R1
                C1
                R2
                C2
        */

        // Define EKF variables
        float delta_t;
        float current_load;
        float voltage_load;

        int temperature;
        int capacity;
        float R0;
        float R1;
        float C1;
        float R2;
        float C2;

    
        // Predict class constructor
        EKFPredict(float dt, float cur, float vol, int temp, int cap, float r0, float r1, float c1, float r2, float c2);
        
        // Class method for obtaining A matrix
        Eigen::Matrix3f getMatrixA(float delta_t, float R1, float C1, float R2, float C2);

    };

} // namespace EKFPred