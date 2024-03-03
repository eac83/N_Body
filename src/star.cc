// Star.cpp
#include "constants.h"
#include "vector_arithmatic.h"
#include "star.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

Star::Star(const double m, const std::vector<double>& x, const std::vector<double>& v, const std::vector<double>& a) {
    mass_ = m;
    position_ = x;
    velocity_ = v;
    acceleration_ = a;
}

void Star::Print(){
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
    std::cout << "Acceleration: ";
    for (int i=0; i<3; ++i) {
        std::cout << acceleration_[i] << ",";
    }
    std::cout << std::endl;
}

void Star::ResetAcceleration() {
    acceleration_ = kZeroVector;
}

void Star::GetAcceleration(Star& star) {
    double distance = sqrt(pow(star.position_[0]-position_[0], 2) + pow(star.position_[1]-position_[1], 2) + pow(star.position_[2]-position_[2], 2));
    std::vector<double> displacement = star.position_ - position_;
    acceleration_ += kG*star.mass_ / pow(distance, 3) * displacement;
}

void Star::UpdateVelocity(const double& kDeltaTime) {
    velocity_ += acceleration_ * kDeltaTime;
}

void Star::UpdatePosition(const double& kDeltaTime) {
    position_ += velocity_ * kDeltaTime;
}

bool Star::WriteOut(const int& snapshot) {
    std::string filename = "../snapshots/snapshot" + std::to_string(snapshot) + ".csv";
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << mass_ << ",";
        file << position_[0] << ",";
        file << position_[1] << ",";
        file << position_[2] << ",";
        file << velocity_[0] << ",";
        file << velocity_[1] << ",";
        file << velocity_[2] << ",";
        file << acceleration_[0] << ",";
        file << acceleration_[1] << ",";
        file << acceleration_[2] << "\n";
        file.close();
        return true;
    } else {
        std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
        return false;
    }
}
