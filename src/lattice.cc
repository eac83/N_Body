#include "star.h"
#include "utils.h"
#include "constants.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
    // Read in parameter file, parameters.txt
    std::string parameter_filename = "../parameters.txt";
    std::unordered_map<std::string, std::string> parameters = ParseParameters(parameter_filename);

    // Assign the parsed parameters
    const double kBoxSize = std::stod(parameters["boxSize"]);

    const int kNumberStars = 10;
    const double kMass = 2e30; // kg

    // Create randomly distributed stars
    std::vector<Star> stars;

    for (int i=0; i<kNumberStars; ++i) {
        for (int j=0; j<kNumberStars; ++j) {
            for (int k=0; k<kNumberStars; ++k) {
                system("clear");
                std::cout << "Star " << i*pow(kNumberStars,2)+j*kNumberStars+k << " / " << pow(kNumberStars, 3) << std::endl;
                std::vector<double> position = {i*kBoxSize/(kNumberStars-1)-kBoxSize/2, j*kBoxSize/(kNumberStars-1)-kBoxSize/2, k*kBoxSize/(kNumberStars-1)-kBoxSize/2};
                stars.emplace_back(kMass, position, kZeroVector, kZeroVector);
            }
        }    
    }

    std::string filename = "../initial_conditions.csv";
    std::ofstream file(filename);
    if (file.is_open()) {
        std::cout << "Writing to " << filename << std::endl;
        file << "m,x,y,z,vx,vy,vz,ax,ay,az\n";
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << " for appending." << std::endl;
    }
    for (int i=0; i<stars.size(); ++i) {
        stars[i].WriteOut(filename);
    }
    return 0;
}
