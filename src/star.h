#ifndef STAR_H_
#define STAR_H_

#include "constants.h"
#include "vector_arithmatic.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

class Star {
    public:
        Star(const double m, const std::vector<double>& x, const std::vector<double>& v, const std::vector<double>& a);

        void Print();
        void ResetAcceleration();
        void GetAcceleration(Star& star);
        void UpdateVelocity(const double& kDeltaTime);
        void UpdatePosition(const double& kDeltaTime);
        bool WriteOut(const int& snapshot);
    
    private:
        double mass_;
        std::vector<double> position_;
        std::vector<double> velocity_;
        std::vector<double> acceleration_;
};

#endif // STAR_H_