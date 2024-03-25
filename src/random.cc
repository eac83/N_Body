#include "particle.h"
#include "utils.h"
#include "constants.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

void PrintHelp(const int& num_particles, const std::string& param_file, const std::string& init_file);

int main(int argc, char* argv[]) {
  // Read in flags
  // Default flag parameters
  int num_particles = 100;
  std::string parameter_filename = "parameters.txt";
  std::string initial_filename = "initial_conditions.csv";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      PrintHelp(num_particles, parameter_filename, initial_filename);
      return 0;
    }
    if (arg == "--number" || arg == "-n") {
      // Check if the flag exists and if the next argument is valid
      if (i + 1 < argc) {
        num_particles = std::stoi(argv[i + 1]);
        if (num_particles <= 0) {
          std::cerr << "Invalid number of particles specified!" << std::endl;
          return 1;
        }
        ++i; // Move to the next argument
      } else {
        std::cerr << "Missing argument for --particles flag!" << std::endl;
        return 1;
      }
    } else if (arg == "--parameters" || arg == "-p") {
      if (i + 1 < argc) {
        parameter_filename = argv[i + 1];
      } else {
        std::cerr << "Missing argument for --parameters flag!" << std::endl;
        return 1;
      }
    } else if (arg == "--initial" || arg == "-i") {
      if (i + 1 < argc) {
        initial_filename = argv[i + 1];
      } else {
        std::cerr << "Missing argument for --initial flag!" << std::endl;
        return 1;
      }
    }
  }

  // Read in parameter file, parameters.txt
  std::unordered_map<std::string, std::string> parameters = ParseParameters(parameter_filename);

  // Assign the parsed parameters
  const double kDMMass = std::stod(parameters["DMMass"]);
  const double kBoxSize = std::stod(parameters["BoxSize"]);

  // Create randomly distributed particles
  std::vector<Particle> particles;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> dist(0, kBoxSize/3);
  std::uniform_real_distribution<double> dist_x(-kBoxSize/2, kBoxSize/2);
  std::uniform_real_distribution<double> dist_y(-kBoxSize/2, kBoxSize/2);
  std::uniform_real_distribution<double> dist_z(-kBoxSize/2, kBoxSize/2);
  for (int i=0; i<num_particles; ++i) {
    std::vector<double> position;
    for (int j=0; j<3; j++) {
      double position_j = dist_x(gen);
      position.push_back(position_j);
    }
    // Apply periodic boundary conditions
    for (int j=0; j<3; ++j) {
        if (position[j] > 0) {
            while (position[j] > kBoxSize / 2) {
                position[j] -= kBoxSize;
            }
        } else {
            while (position[j] < -kBoxSize / 2) {
                position[j] += kBoxSize;
            }
        }
    }
    particles.emplace_back(kDMMass, position, kZeroVector);
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

  for (int i=0; i<num_particles; ++i) {
    particles[i].WriteOut(filename);
  }
  return 0;
}


void PrintHelp(const int& num_particles, const std::string& param_file, const std::string& init_file) {
  std::cout << "Usage: main [OPTIONS]\n"
            << "Options:\n"
            << "    --number, -n <num>         Number of particles to seed (default: "<<  num_particles << ")\n"
            << "    --parameters, -p <file>    Path to parameter file (default: " << param_file << ")\n"
            << "    --initials, -i <file>      Path to initial conditions file (default: " << init_file << ")\n"
            << "    --help, -h                 Display this help message and exit\n";
}
