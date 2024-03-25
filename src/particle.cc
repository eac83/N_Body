// particle.cc
#include "constants.h"
#include "vector_arithmatic.h"
#include "particle.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

Particle::Particle(const double m, const std::vector<double>& x, const std::vector<double>& v) {
  mass_ = m;
  position_ = x;
  velocity_ = v;
}

void Particle::Print(){
  std::cout << "Mass: " << mass_ << std::endl;
  std::cout << "Position: ";
  for (int i=0; i<3; ++i) {
    std::cout << position_[i] << ",";
  }
  std::cout << std::endl;
  std::cout << "Velocity: ";
  for (int i=0; i<3; ++i) {
    std::cout << velocity_[i] << ",";
  }
  std::cout << std::endl;
}

void Particle::ResetAcceleration() {
  k_v_1_ = kZeroVector;
  k_v_2_ = kZeroVector;
  k_v_3_ = kZeroVector;
  k_v_4_ = kZeroVector;
}

std::vector<double> Particle::GetAcceleration(Particle& Particle, const double& box_size, const double& G, const double& softening_length, const std::vector<double>& k) {
  //std::cout << "Calculating displacement..." << std::endl;
  std::vector<double> displacement = GetDisplacement(Particle, box_size, k);
  //std::cout << "Calculating distance..." << std::endl;
  double distance = sqrt(Dot(displacement, displacement));
  //std::cout << "Returning force." << std::endl;
  return G*Particle.mass_ / (pow(distance, 2) + pow(softening_length, 2)) * (displacement / distance);
}

void Particle::UpdateVelocity(const double& kTimeStep) {
  velocity_ += kTimeStep/6 * (k_v_1_ + (2.*k_v_2_) + (2.*k_v_3_) + k_v_4_);
}

void Particle::UpdatePosition(const double& kTimeStep, const double& box_size) {
  position_ += kTimeStep/6 * (k_x_1_ + (2.*k_x_2_) + (2. *k_x_3_) + k_x_4_);
  for (int i=0; i<3; ++i) {
    if (position_[i] > 0) {
      while (position_[i] > box_size / 2) {
        position_[i] -= box_size;
      }
    } else {
      while (position_[i] < -box_size / 2) {
        position_[i] += box_size;
      }
    }
  }
}

bool Particle::WriteOut(const std::string& filename) {
  std::ofstream file(filename, std::ios::app);
  if (file.is_open()) {
    file << mass_ << ",";
    file << position_[0] << ",";
    file << position_[1] << ",";
    file << position_[2] << ",";
    file << velocity_[0] << ",";
    file << velocity_[1] << ",";
    file << velocity_[2] << "\n";
    file.close();
    return true;
  } else {
    std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
    return false;
  }
}

std::vector<double> Particle::GetDisplacement(Particle& Particle, const double& box_size, const std::vector<double> k) {
  std::vector<double> displacement = Particle.position_ - (position_ + k);
  for (int i=0; i<3; ++i) {
    if (displacement[i] > box_size / 2) {
      displacement[i] -= box_size;
    }
    if (displacement[i] < -box_size / 2) {
      displacement[i] += box_size;
    }
  }
  return displacement;
}

void Particle::Expand(double a, double a_old) {
  velocity_ *= a_old / a;
}

void Particle::GetKV(const int& i, Particle& Particle, const double& box_size, const double& G, const double& softening_length, const double& time_step) {
  switch (i) {
    case 1:
      //std::cout << "Calculating k_v_1..." << std::endl;
      k_v_1_ += GetAcceleration(Particle, box_size, G, softening_length, kZeroVector);
      break;
    case 2:
    //  std::cout << "Calculating k_v_2..." << std::endl;
      k_v_2_ += GetAcceleration(Particle, box_size, G, softening_length, k_x_1_*(time_step/2));
      break;
    case 3:
      //std::cout << "Calculating k_v_3..." << std::endl;
      k_v_3_ += GetAcceleration(Particle, box_size, G, softening_length, k_x_2_*(time_step/2));
      break;
    case 4:
      //std::cout << "Calculating k_v_4..." << std::endl;
      k_v_4_ += GetAcceleration(Particle, box_size, G, softening_length, k_x_3_*time_step);
      break;
  }
}

void Particle::GetKX(const int& i, const double& time_step) {
  switch (i) {
    case 1:
    //  std::cout << "Calculating k_x_1..." << std::endl;
      k_x_1_ = velocity_;
      break;
    case 2:
    //  std::cout << "Calculating k_x_2..." << std::endl;
      k_x_2_ = velocity_ + k_v_1_ * (time_step/2);
      break;
    case 3:
    //  std::cout << "Calculating k_x_3..." << std::endl;
      k_x_3_ = velocity_ + k_v_2_ * (time_step/2);
      break;
    case 4:
    //  std::cout << "Calculating k_x_4..." << std::endl;
      k_x_4_ = velocity_ + k_v_3_ * time_step;
      break;
  }
}