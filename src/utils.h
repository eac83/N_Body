// utils.h
#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

std::unordered_map<std::string, std::string> ParseParameters(const std::string& filename);

#endif