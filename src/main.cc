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

void PrintHelp(const int& max_threads, const std::string& param_file, const std::string& init_file);

int main(int argc, char* argv[]) {
  // Read in flags
  // Default flag parameters
  int num_threads = omp_get_max_threads();
  std::string parameter_filename = "parameters.txt";
  std::string initial_filename = "initial_conditions.csv";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      PrintHelp(num_threads, parameter_filename, initial_filename);
      return 0;
    }
    if (arg == "--threads" || arg == "-t") {
      // Check if the flag exists and if the next argument is valid
      if (i + 1 < argc) {
        num_threads = std::stoi(argv[i + 1]);
        if (num_threads <= 0) {
          std::cerr << "Invalid number of threads specified!" << std::endl;
          return 1;
        }
        ++i; // Move to the next argument
      } else {
        std::cerr << "Missing argument for --threads flag!" << std::endl;
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

  // Set the number of threads
  omp_set_num_threads(num_threads);

  // Read in parameter file, parameters.txt
  std::unordered_map<std::string, std::string> parameters = ParseParameters(parameter_filename);

  // Assign the parsed parameters
  const double kDeltaTime = std::stod(parameters["deltaTime"]);
  const double kMaxTime = std::stod(parameters["maxTime"]);
  const int kStepsBetweenSnapshots = std::stoi(parameters["stepsBetweenSnapshots"]);
  const double kBoxSize = std::stod(parameters["boxSize"]);
  const double kSofteningLength = std::stod(parameters["softeningLength"]);
  
  // Read in initial conditions
  std::ifstream file_initial(initial_filename);
  if(!file_initial.is_open()) {
    std::cerr << "Faliled to open the file " << initial_filename << "." << std::endl;
    return 1;
  }

  // Create stars from initial conditions
  std::vector<Star> stars;
  std::string line;
  std::getline(file_initial, line);

  while (std::getline(file_initial, line)) {
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
  
  file_initial.close();
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

void PrintHelp(const int& max_threads, const std::string& param_file, const std::string& init_file) {
  std::cout << "Usage: main [OPTIONS]\n"
            << "Options:\n"
            << "    --threads, -t <num>    Number of threads to use (default: maximum available threads (currently: " << max_threads << ")\n"
            << "    --parameters, -p <file>    Path to parameter file (default " << param_file << ")\n"
            << "    --initials, -i <file>    Path to initial conditions file (default" << init_file << ")\n"
            << "    --help, -h    Display this help message and exit\n";
}