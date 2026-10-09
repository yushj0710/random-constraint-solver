#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

namespace Utils {

// Function to read a file and return its contents as a string
std::string readFile(const std::string& filename);

// Function to split a string by a delimiter
std::vector<std::string> split(const std::string& str, char delimiter);

// Function to trim whitespace from both ends of a string
std::string trim(const std::string& str);

} // namespace Utils

#endif // UTILS_HPP
