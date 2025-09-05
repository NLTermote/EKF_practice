/**
 * @brief CSV parser header file, used for loading CSV vectors
 *
 * This file contains the header for the class for the CSV parser
 *
 * @author Nils Termote
 * @date 2024-04-09
*/

#pragma once

#include <Eigen/Dense>
#include <string>
#include <vector>
#include <chrono>


namespace CSVP {

class CSVParser {
public:

    std::string filePath;
    std::vector<std::string> timestamps_strs;
    std::vector<std::chrono::system_clock::time_point> timestamps;
    std::vector<float> voltages;
    std::vector<float> currents;

    // constructor for filepath
    CSVParser(std::string input);

    void readCSV();

    void printFilePath();

    void printVectorSizes();

    std::vector<float> getVoltages();

    std::vector<float> getCurrents();
};

} // namespace CSVP