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

void Star::GetAcceleration(Star& star, const double& box_size, const double& softening_length) {
    std::vector<double> displacement = GetDisplacement(star, box_size);
    //std::cout << displacement[0] << ", " << displacement[1] << ", " << displacement[2] << std::endl;
    double distance = sqrt(Dot(displacement, displacement));
    //std::cout << distance << std::endl;

    acceleration_ += kG*star.mass_ / (pow(distance, 2) + pow(softening_length, 2)) * (displacement / distance);
}

void Star::UpdateVelocity(const double& kDeltaTime) {
    velocity_ += acceleration_ * kDeltaTime;
}

void Star::UpdatePosition(const double& kDeltaTime, const double& box_size) {
    position_ += velocity_ * kDeltaTime;
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

bool Star::WriteOut(const std::string& filename) {
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

std::vector<double> Star::GetDisplacement(Star& star, const double& box_size) {
    std::vector<double> displacement = star.position_ - position_;
    for (int i=0; i<3; ++i) {
        if (displacement[i] > box_size / 2) {
            displacement[i] -= box_size / 2;
        }
        if (displacement[i] < -box_size / 2) {
            displacement[i] += box_size / 2;
        }
    }
    return displacement;
}