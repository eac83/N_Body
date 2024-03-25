// expansion.cc
#include <iostream>
#include <cmath>

double Friedmann(const double& scale_factor, const double& Omega_matter, const double& Omega_dark_energy) {
  return Omega_matter / scale_factor + Omega_dark_energy * pow(scale_factor,2);
}

double Expand(const double& scale_factor, const double& H_0, const double& time_step, const double& Omega_matter, const double& Omega_dark_energy) {
  double k_1 = H_0 * sqrt(Friedmann(scale_factor, Omega_matter, Omega_dark_energy));
  double k_2 = H_0 * sqrt(Friedmann(scale_factor + time_step*k_1/2, Omega_matter, Omega_dark_energy));
  double k_3 = H_0 * sqrt(Friedmann(scale_factor + time_step*k_2/2, Omega_matter, Omega_dark_energy));
  double k_4 = H_0 * sqrt(Friedmann(scale_factor + time_step*k_3, Omega_matter, Omega_dark_energy));
  return scale_factor + time_step/6 * (k_1 + 2*k_2 + 2*k_3 + k_4);
}