#ifndef Particle_H_
#define Particle_H_

#include "constants.h"
#include "vector_arithmatic.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

class Particle {
  public:
    Particle(const double m, const std::vector<double>& x, const std::vector<double>& v);
    void Print();
    void ResetAcceleration();
    std::vector<double> GetAcceleration(Particle& Particle, const double& box_size, const double& G, const double& softening_length, const std::vector<double>& k);
    void UpdateVelocity(const double& kTimeStep);
    void UpdatePosition(const double& kTimeStep, const double& box_size);
    bool WriteOut(const std::string& filename);
    std::vector<double> GetDisplacement(Particle& Particle, const double& box_size, const std::vector<double> k);
    void Expand(double a, double a_old);
    void GetKV(const int& i, Particle& Particle, const double& box_size, const double& G, const double& softening_length, const double& time_step);
    void GetKX(const int& i, const double& time_step);

  private:
    double mass_;
    std::vector<double> position_;
    std::vector<double> velocity_;
    std::vector<double> k_v_1_;
    std::vector<double> k_v_2_;
    std::vector<double> k_v_3_;
    std::vector<double> k_v_4_;
    std::vector<double> k_x_1_;
    std::vector<double> k_x_2_;
    std::vector<double> k_x_3_;
    std::vector<double> k_x_4_;
};

#endif // particle_H_