#include "utils.h"
#include "vector_arithmatic.h"
#include "constants.h"
#include "star.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <omp.h>
#include <filesystem>

int main() {
  // Read in parameter file, parameters.txt
  std::string parameter_filename = "parameters.txt";
  std::unordered_map<std::string, std::string> parameters = ParseParameters(parameter_filename);

  // Assign the parsed parameters
  const double kDeltaTime = std::stod(parameters["deltaTime"]);
  const double kMaxTime = std::stod(parameters["maxTime"]);
  const int kStepsBetweenSnapshots = std::stoi(parameters["stepsBetweenSnapshots"]);
  const double kBoxSize = std::stod(parameters["boxSize"]);
  const double kSofteningLength = std::stod(parameters["softeningLength"]);
  
  // Read in initial conditions
  std::string input_filename = "initial_conditions.csv";
  std::ifstream file_input(input_filename);
  if(!file_input.is_open()) {
    std::cerr << "Faliled to open the file " << input_filename << "." << std::endl;
    return 1;
  }

  // Create stars from initial conditions
  std::vector<Star> stars;
  std::string line;
  std::getline(file_input, line);

  while (std::getline(file_input, line)) {
    double m, x, y, z, vx, vy, vz, ax, ay, az;
    char comma;
    if (std::istringstream(line) >> m >> comma >> x >> comma >> y >> comma >> z >> comma >> vx >> comma >> vy >> comma >> vz >> comma >> ax >> comma >> ay >> comma >> az) {
      std::vector<double> pos = {x, y, z};
      std::vector<double> vel = {vx, vy, vz};
      std::vector<double> acc = {ax, ay, az};
      stars.emplace_back(m, pos, vel, acc);
    } else {
      std::cerr << "Invalid data format: " << line << std::endl;
      return 1;
    }
  }
  
  file_input.close();
  const int kNumberStars = stars.size();

  // Initialize variables for keeping track of steps.
  int snapshot = 0;
  int step = 0;
  double time = 0;

  // Step 0
  std::string filename = "snapshots/snapshot" + std::to_string(snapshot) + ".csv";
  std::ofstream file(filename);
  if (file.is_open()) {
    file << "m,x,y,z,vx,vy,vz,ax,ay,az\n";
    file.close();
  } else {
    std::cerr << "Unable to open file " << filename << " for appending." << std::endl;
  }
  for (int i=0; i<kNumberStars; ++i) {
    stars[i].WriteOut(filename);
  }

  // Step forward loop
  const int kProgressBarWidth = 50;
  const int kTotalSteps = int(kMaxTime/kDeltaTime) + 1;
  while (time < kMaxTime) {
    time += kDeltaTime;
    ++step;
    
    // Progress bar
    int progress_bar_position = int(kProgressBarWidth * step / kTotalSteps);

    std::cout << "[";
    for (int i=0; i < kProgressBarWidth; ++i) {
      if (i < progress_bar_position) {
        std::cout << "=";
      } else if (i == progress_bar_position) {
        std::cout << ">";
      } else {
        std::cout << "-";
      }
    }
    std::cout << "] " << step << "/" << kTotalSteps << "\r";
    std::cout.flush();
    // Reset acceleration to (0, 0, 0)
    #pragma omp parallel for
    for (int i=0; i<kNumberStars; ++i) {
      //   std::cout << "Resetting star " << i << "'s acceleration..." << std::endl;
        stars[i].ResetAcceleration();
    }
    // Calculate force from all other stars
    #pragma omp parallel for
    for (int i=0; i<kNumberStars; ++i) {
        for (int j=0; j<kNumberStars; j++) {
            if (i != j) {
                //std::cout << "From star " << j << "..." << std::endl;
                stars[i].GetAcceleration(stars[j], kBoxSize, kSofteningLength);
            }
        }
    }
    // Step forward star velocities
    #pragma omp parallel for
    for (int i=0; i<kNumberStars; ++i) {
        // std::cout << "Updating star " << i << "'s velocity..." << std::endl;
        stars[i].UpdateVelocity(kDeltaTime);
    }
    // Step forward star positions
    #pragma omp parallel for
    for (int i=0; i<kNumberStars; ++i) {
        // std::cout << "Updating star " << i << "'s position ..." << std::endl;
        stars[i].UpdatePosition(kDeltaTime, kBoxSize);
    }
    // std::cout << "Time: " << time << std::endl;
    // #pragma omp parallel for
    // for (int i=0; i<kNumberStars; ++i) {
        // stars[i].Print();
    // }
    // Save snapshot
    if (step % kStepsBetweenSnapshots == 0) {
        snapshot += 1;
        std::string filename = "snapshots/snapshot" + std::to_string(snapshot) + ".csv";
        std::ofstream file(filename);
        if (file.is_open()) {
            // std::cout << "Writing to " << filename << std::endl;
            file << "m,x,y,z,vx,vy,vz,ax,ay,az\n";
            file.close();
        } else {
            std::cerr << "Unable to open file " << filename << " for appending." << std::endl;
        }
        for (int i=0; i<kNumberStars; ++i) {
            stars[i].WriteOut(filename);
        }
    }
  }
  std::cout << std::endl;

  return 0;
}