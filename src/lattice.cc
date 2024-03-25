#include "particle.h"
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

    const int kNumberParticles = 10;
    const double kMass = 2e30; // kg

    // Create particles distributed in a 3d grid
    std::vector<Particle> particles;
    const int kProgressBarWidth = 50;
    const int kTotalNumberParticles = pow(kNumberParticles,3);
    int step = 0;
    for (int i=0; i<kNumberParticles; ++i) {
        for (int j=0; j<kNumberParticles; ++j) {
            for (int k=0; k<kNumberParticles; ++k) {
                ++step;
                int progress_bar_position = int(kProgressBarWidth * step / kTotalNumberParticles);
                std::cout << "[";
                for (int l=0; l < kProgressBarWidth; ++l) {
                    if (l < progress_bar_position) {
                        std::cout << "=";
                    } else if (l == progress_bar_position) {
                        std::cout << ">";
                    } else {
                        std::cout << "-";
                    }
                }
                std::cout << "] " << step << "/" << kTotalNumberParticles << "\r";
                std::cout.flush();

                std::vector<double> position = {i*kBoxSize/(kNumberParticles-1)-kBoxSize/2, j*kBoxSize/(kNumberParticles-1)-kBoxSize/2, k*kBoxSize/(kNumberParticles-1)-kBoxSize/2};
                particles.emplace_back(kMass, position, kZeroVector);
            }  
        }    
    }
    std::cout << std::endl;
    std::string filename = "initial_conditions.csv";
    std::ofstream file(filename);
    if (file.is_open()) {
        std::cout << "Writing to " << filename << std::endl;
        file << "m,x,y,z,vx,vy,vz,ax,ay,az\n";
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << " for appending." << std::endl;
    }
    for (unsigned long i=0; i<particles.size(); ++i) {
        particles[i].WriteOut(filename);
    }

    
    return 0;
}
