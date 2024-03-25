#ifndef Expansion_H_
#define Expansion_H_

#include <iostream>
#include <cmath>

double Friedmann(const double& scale_factor, const double& Omega_matter, const double& Omega_dark_energy);

double Expand(const double& scale_factor, const double& H_0, const double& time_step, const double& Omega_matter, const double& Omega_dark_energy);

#endif // Expanson_H_