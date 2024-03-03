#ifndef VECTOR_ARITHMATIC_H
#define VECTOR_ARITHMATIC_H

#include <iostream>
#include <vector>

// Scalar and std::vector

// Scalar multiplication
template<typename T>
std::vector<T> operator*(const T& s, const std::vector<T>& v) {
    std::vector<T> result;
    result.reserve(v.size());

    for (size_t i = 0; i < v.size(); ++i) {
        result.push_back(s * v[i]);
    }

    return result;
}

template<typename T>
std::vector<T> operator*(const std::vector<T>& v, const T& s) {
    return s * v;
}


// std::vector and std::vector

// Vector addition
template<typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors was have same size for element-wise addition.");
    }

    std::vector<T> result;
    result.reserve(a.size());

    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(a[i] + b[i]);
    }

    return result;
}

// Vector subtraction
template<typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors was have same size for element-wise subtraction.");
    }

    std::vector<T> result;
    result.reserve(a.size());

    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(a[i] - b[i]);
    }

    return result;
}

// Vector addition assignment
template<typename T>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors was have same size for element-wise addition assignment.");
    }
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] += b[i];
    }
    return a;
}

#endif // VECTOR_ARITHMATIC_H