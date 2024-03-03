#include "utils.h"

// Create map from parameter file
std::unordered_map<std::string, std::string> ParseParameters(const std::string& filename) {
    std::unordered_map<std::string, std::string> parameters;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file " << filename << "." << std::endl;
        return parameters; // Return empty map if file opening fails
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines or lines starting with '#'
        if (line.empty() || line[0] == '#')
            continue;

        // Find the position of the first '=' character
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            // Extract key and value
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // Remove leading and trailing whitespaces from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Add key-value pair to the map
            parameters[key] = value;
        }
    }

    file.close();
    return parameters;
}
