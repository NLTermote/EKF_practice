/**
 * @brief Entry point for the application.
 *
 * This file contains the main function which initializes and starts the application.
 *
 * @author Nils Termote
 * @date 2024-03-09
*/

#include <iostream>
#include <string>
//#include <vector>
//#include <fstream>
//#include <sstream>
//#include <filesystem>







int main (int argc, char **argv) {
    CSVParser EKF(argv[1]);
    EKF.readCSV();
    EKF.printFilePath();
    EKF.printVectorSizes();


    /*
    for each csv line:

        run_estimator();
            return SOC vector
    */


    return 0;
}
    
// /home/ntermote/Documents/ExR_files/estimator/data/ccd_cycle_1a.csv"



