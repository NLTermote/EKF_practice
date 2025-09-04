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

#include "../include/exp_helpers.hpp"



class CSVParser {
    public:
    
        std::string filePath;

        std::vector<std::string> timestamps_strs;
        std::vector<std::chrono::system_clock::time_point> timestamps;
        std::vector<float> voltages;
        std::vector<float> currents;

        // constructor for filepath
        CSVParser(std::string input) : filePath(input) {}

        void readCSV() {
            //std::filesystem::path(filePath).stem();

            csv::CSVFormat format;
            format.delimiter(',').header_row(0);  // The first row is the header row
            csv::CSVReader reader(filePath, format);
            
            // Vectors to store parsed data

            //std::vector<float> soc_actual;

            // Iterate through rows
            for (csv::CSVRow &row : reader)
            {
                std::string timeStr = row["timestamp"];
                std::string voltStr = row["voltage_load"];
                std::string currStr = row["current_load"];
                //std::string socStr = row["soc_actual"];

                if (timeStr.empty() || voltStr.empty() || currStr.empty() /*|| socStr.empty()*/)
                {
                    std::cerr << "Invalid columns provided in csv parsing";
                    exit(1);
                }

                // Convert text to appropriate types
                std::chrono::time_point tp = parseTimestamp(timeStr);
                float vol = std::stof(voltStr);
                float cur = std::stof(currStr);
                //float soc = std::stof(socStr);

                // Store data
                timestamps_strs.push_back(timeStr);
                timestamps.push_back(tp);
                voltages.push_back(vol);
                currents.push_back(cur);
                //soc_actual.push_back(soc);
            }


            std::cout << "succeeded" << std::endl;
        };

        void printFilePath() {
            std::cout << "The filepath is: " << filePath << std::endl;
        }

        void printVectorSizes() {
            std::cout << "timestamp size: " << timestamps.size() << std::endl;
            std::cout << "currents size: " << currents.size() << std::endl;
            std::cout << "voltages size: " << voltages.size() << std::endl;
        };

        
};





int main (int argc, char **argv) {
   
    CSVParser test(argv[1]);
    test.readCSV();
    test.printFilePath();
    test.printVectorSizes();

    return 0;
}

// /home/ntermote/Documents/ExR_files/estimator/data/ccd_cycle_1a.csv"



