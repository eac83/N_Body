#include "utils.h"
#include "vector_arithmatic.h"
#include "constants.h"
#include "particle.h"
#include "expansion.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <omp.h>
#include <filesystem>
#include <chrono>

void PrintHelp(const int& max_threads, const std::string& param_file, const std::string& init_file);

int main(int argc, char* argv[]) {
  // Read in flags
  // Default flag parameters
  int num_threads = omp_get_max_threads();
  std::string parameter_filename = "parameters.txt";
  std::string initial_filename = "initial_conditions.csv";
  bool isContinue = false;
  int snapshot = 0;

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
    } else if (arg == "--continue" || arg == "-c") {
      isContinue = true;
      if (i + 1 < argc) {
        snapshot = std::stoi(argv[i + 1]);
        if (snapshot < 0) {
          std::cout << "Invalid snapshot number specified!" << std::endl;
          return 1;
        }
      }
    }
  } 

  // Set the number of threads
  omp_set_num_threads(num_threads);
  std::cout << "Thread count: " << num_threads << std::endl;
  // Read in parameter file, parameters.txt
  std::unordered_map<std::string, std::string> parameters = ParseParameters(parameter_filename);

  // Assign the parsed parameters
  // Code units
  const double kMassUnit = std::stod(parameters["MassUnit"]);
  const double kLengthUnit = std::stod(parameters["LengthUnit"]);
  const double kVelocityUnit = std::stod(parameters["VelocityUnit"]);

  // Code parameters
  const double kTimeStep = std::stod(parameters["TimeStep"]);
  const double kMaxTime = std::stod(parameters["MaxTime"]);
  const int kStepsBetweenSnapshots = std::stoi(parameters["StepsBetweenSnapshots"]);
  const double kBoxSize = std::stod(parameters["BoxSize"]);
  const double kSofteningLength = std::stod(parameters["SofteningLength"]);

  // Cosmological parameters
  const double kOmegaMatter = std::stod(parameters["OmegaM"]);
  const double kOmegaDarkEnergy = std::stod(parameters["OmegaL"]);
  const double kh = std::stod(parameters["h"]);
  double scale_factor = std::stod(parameters["InitialScaleFactor"]);

  // Composed parameters
  const double kH0 = 100 * kh * 1e5 / (kpc_CGS * 1e6);  // s^-1

  // Scale constants by code parameters
  double G_units = kG_CGS * kMassUnit / kLengthUnit / pow(kVelocityUnit,2) / pow(scale_factor,3);

  // Initialize variables for keeping track of steps.
  int step = 0;
  double time = 0;

  if (isContinue == true) {
    initial_filename = "./snapshots/snapshot"+std::to_string(snapshot)+".csv";
    std::string continue_filename = "./snapshots/state" + std::to_string(snapshot) + ".txt";
    std::unordered_map<std::string, std::string> continue_parameters = ParseParameters(continue_filename);
    time = std::stod(continue_parameters["Time"]);
    step = std::stoi(continue_parameters["Step"]);
    scale_factor = std::stod(continue_parameters["ScaleFactor"]);
  } else {
    std::cout << "Reading in initial conditions..." << std::endl;
  }

  // Read in initial conditions
  std::ifstream file_initial(initial_filename);
  if(!file_initial.is_open()) {
    std::cerr << "Faliled to open the file " << initial_filename << "." << std::endl;
    return 1;
  }
  
  // Create particles from initial conditions
  std::vector<Particle> particles;
  std::string line;
  std::getline(file_initial, line);

  while (std::getline(file_initial, line)) {
    double m, x, y, z, vx, vy, vz;
    char comma;
    if (std::istringstream(line) >> m >> comma >> x >> comma >> y >> comma >> z >> comma >> vx >> comma >> vy >> comma >> vz) {
      std::vector<double> pos = {x, y, z};
      std::vector<double> vel = {vx, vy, vz};
      particles.emplace_back(m, pos, vel);
    } else {
      std::cerr << "Invalid data format: " << line << std::endl;
      return 1;
    }
  }
  file_initial.close();
  const int kNumberParticles = particles.size();


  // Empty snapshots directory if not continuing
  if (isContinue == false) {
    std::system("rm ./snapshots/*");

    // Step 0
    std::cout << "Writing out initial snapshot..." << std::endl;
    std::string filename = "snapshots/snapshot" + std::to_string(snapshot) + ".csv";
    std::ofstream file(filename);
    if (file.is_open()) {
      file << "m,x,y,z,vx,vy,vz,ax,ay,az\n";
      file.close();
    } else {
      std::cerr << "Unable to open file " << filename << " for appending." << std::endl;
    }
    for (int i=0; i<kNumberParticles; ++i) {
      particles[i].WriteOut(filename);
    }
    std::string state_filename = "snapshots/state" + std::to_string(snapshot) + ".txt";
    std::ofstream state_file(state_filename);
    if (state_file.is_open()) {
      state_file << "Time=" << time << "\n";
      state_file << "Step=" << step << "\n";
      state_file << "ScaleFactor=" << scale_factor << "\n";
      state_file.close();
    } else {
      std::cerr << "Unable to open file " << state_filename << " for appending." << std::endl;
    }
  }

  // Step forward loop
  const int kProgressBarWidth = 50;
  const int kTotalSteps = int(kMaxTime/kTimeStep) + 1;

  // Start time
  if (isContinue == true) {
    std::cout << "Continuing simulation." << std::endl;
  } else {
  std::cout << "Beginning simulation." << std::endl;
  }
  std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();
  while (time < kMaxTime) {
    std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
    time += kTimeStep;
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
    std::cout << "] " << step << "/" << kTotalSteps << ", " << std::chrono::duration_cast<std::chrono::minutes>(time_now - time_begin).count() << " min /" << static_cast<double>(std::chrono::duration_cast<std::chrono::minutes>((time_now - time_begin) ).count())* kTotalSteps / (step - 1) << " min" << "\r";
    std::cout.flush();
    // Reset acceleration to (0, 0, 0)

    #pragma omp parallel for
    for (int i=0; i<kNumberParticles; ++i) {
      particles[i].ResetAcceleration();
    }

    // Update velocities and positions using RK4
  //  std::cout << "Performing RK4..." << std::endl;
    for (int i=1; i<=4; ++i) {
      // Get k_v_i
    //  std::cout << "k_v_" << i << "..." << std::endl;
      #pragma omp parallel for
      for (int j=0; j<kNumberParticles; ++j) {
    //    std::cout << "Force on " << j;
        for (int k=0; k<kNumberParticles; ++k) {
          if (j != k) {
//std::cout << " from " << k << "..." << std::endl;
            particles[j].GetKV(i, particles[k], kBoxSize, G_units, kSofteningLength, kTimeStep);
          } 
        }
      }
      // Get k_x_i
    //  std::cout << "k_x_" << i << "..." << std::endl;
      #pragma omp parallel for
      for (int j=0; j<kNumberParticles; ++j) {
        particles[j].GetKX(i, kTimeStep);
      }
    }
    // Update final velocities and positions
//std::cout << "Updating velocities and positions..." << std::endl;
    for (int i=1; i<kNumberParticles; ++i) {
      particles[i].UpdateVelocity(kTimeStep);
      particles[i].UpdatePosition(kTimeStep, kBoxSize);
    } 
    // Save snapshot and state files
    if (step % kStepsBetweenSnapshots == 0) {
      snapshot += 1;
      std::string filename = "snapshots/snapshot" + std::to_string(snapshot) + ".csv";
      std::ofstream file(filename);
      if (file.is_open()) {
        file << "m,x,y,z,vx,vy,vz,ax,ay,az\n";
        file.close();
      } else {
        std::cerr << "Unable to open file " << filename << " for appending." << std::endl;
      }
      for (int i=0; i<kNumberParticles; ++i) {
        particles[i].WriteOut(filename);
      }
      std::string state_filename = "snapshots/state" + std::to_string(snapshot) + ".txt";
      std::ofstream state_file(state_filename);
      if (state_file.is_open()) {
        state_file << "Time=" << time << "\n";
        state_file << "Step=" << step << "\n";
        state_file << "ScaleFactor=" << scale_factor << "\n";
        state_file.close();
      } else {
        std::cerr << "Unable to open file " << state_filename << " for appending." << std::endl;
      }
    }

    // Perform cosmic expansion
    double scale_factor_old = scale_factor;
    scale_factor = Expand(scale_factor, kH0, kTimeStep, kOmegaMatter, kOmegaDarkEnergy);
    G_units = kG_CGS * kMassUnit / kLengthUnit / pow(kVelocityUnit,2) / pow(scale_factor,3);
    #pragma omp parallel for
    for (int i=0; i<kNumberParticles; ++i) {
      particles[i].Expand(scale_factor, scale_factor_old);
    }
  }
  std::cout << std::endl;

  return 0;
}

void PrintHelp(const int& max_threads, const std::string& param_file, const std::string& init_file) {
  std::cout << "Usage: main [OPTIONS]\n"
            << "Options:\n"
            << "    --threads, -t <num>        Number of threads to use (default: maximum available threads, currently: " << max_threads << ")\n"
            << "    --parameters, -p <file>    Path to parameter file (default: " << param_file << ")\n"
            << "    --initials, -i <file>      Path to initial conditions file (default: " << init_file << ")\n"
            << "    --continue, -c <num>       Snapshot number from which to continue simulation (default: 0)\n"
            << "    --help, -h                 Display this help message and exit\n";
}