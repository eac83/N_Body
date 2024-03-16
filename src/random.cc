#include "star.h"
#include "utils.h"
#include "constants.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
    // Read in parameter file, parameters.txt
    std::string parameter_filename = "parameters.txt";
    std::unordered_map<std::string, std::string> parameters = ParseParameters(parameter_filename);

    // Assign the parsed parameters
    const double kBoxSize = std::stod(parameters["boxSize"]);

    const int kNumberStars = 100;
    const double kMass = 2e30; // kg

    // Create randomly distributed stars
    std::vector<Star> stars;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-kBoxSize/2, kBoxSize/2);

    for (int i=0; i<kNumberStars; ++i) {
        std::vector<double> position;
        for (int j=0; j<3; j++) {
            double position_j = dist(gen);
            position.push_back(position_j);
        }
        stars.emplace_back(kMass, position, kZeroVector, kZeroVector);
    }
    std::string filename = "initial_conditions.csv";
    std::ofstream file(filename);
    if (file.is_open()) {
        std::cout << "Writing to " << filename << std::endl;
        file << "m,x,y,z,vx,vy,vz,ax,ay,az\n";
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << " for appending." << std::endl;
    }
    for (int i=0; i<kNumberStars; ++i) {
        stars[i].WriteOut(filename);
    }
    return 0;
}
