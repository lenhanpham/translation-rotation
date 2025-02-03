#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// Trim whitespace from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

// Read XYZ file
void read_xyz(const std::string& file_path, std::vector<Point>& points, int& num_points) {
    std::ifstream file(file_path);
    if (!file) throw std::runtime_error("Error: Unable to open file " + file_path);
    std::string line;
    num_points = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Point p;
        if (!(iss >> p.name >> p.x >> p.y >> p.z)) continue;  // Skip invalid lines
        points.push_back(p);
        num_points++;
    }
}

// Write XYZ file (buffered output for efficiency)
void write_xyz(const std::string& file_path, const std::vector<Point>& points) {
    std::ofstream file(file_path);
    if (!file) throw std::runtime_error("Error: Unable to write to file " + file_path);
    file << points.size() << "\nTranslated and rotated structure\n";
    for (const auto& p : points) {
        file << p.name << " " << p.x << " " << p.y << " " << p.z << "\n";
    }
}

// Calculate Euclidean distance using Eigen
double calculate_distance(const Point& p1, const Point& p2) {
    Eigen::Vector3d v1(p1.x, p1.y, p1.z);
    Eigen::Vector3d v2(p2.x, p2.y, p2.z);
    return (v1 - v2).norm();
}

// Check minimum distance condition
bool check_min_distance(const std::vector<Point>& points1, const std::vector<Point>& points2, double min_dist) {
    for (const auto& p1 : points1) {
        for (const auto& p2 : points2) {
            if (calculate_distance(p1, p2) < min_dist) return false;
        }
    }
    return true;
}

// Combined translation and rotation
void translate_rotate_points(std::vector<Point>& points, const Eigen::Vector3d& direction, double delta_s,
                             const Eigen::Vector3d& axis, double angle) {
    Eigen::Matrix3d R;
    double cos_angle = std::cos(angle);
    double sin_angle = std::sin(angle);

    R << cos_angle + axis.x() * axis.x() * (1 - cos_angle),
         axis.x() * axis.y() * (1 - cos_angle) - axis.z() * sin_angle,
         axis.x() * axis.z() * (1 - cos_angle) + axis.y() * sin_angle,

         axis.y() * axis.x() * (1 - cos_angle) + axis.z() * sin_angle,
         cos_angle + axis.y() * axis.y() * (1 - cos_angle),
         axis.y() * axis.z() * (1 - cos_angle) - axis.x() * sin_angle,

         axis.z() * axis.x() * (1 - cos_angle) - axis.y() * sin_angle,
         axis.z() * axis.y() * (1 - cos_angle) + axis.x() * sin_angle,
         cos_angle + axis.z() * axis.z() * (1 - cos_angle);

#pragma omp parallel for schedule(guided)
    for (size_t i = 0; i < points.size(); ++i) {
        Eigen::Vector3d pos(points[i].x, points[i].y, points[i].z);
        pos += direction * delta_s;
        pos = R * pos;
        points[i].x = pos(0);
        points[i].y = pos(1);
        points[i].z = pos(2);
    }
}

// Parse input file
std::unordered_map<std::string, std::string> parse_input_file(const std::string& input_file) {
    std::unordered_map<std::string, std::string> params;
    std::ifstream file(input_file);
    if (!file) throw std::runtime_error("Error: Unable to open input file " + input_file);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::replace(line.begin(), line.end(), ':', ' ');
        std::replace(line.begin(), line.end(), '=', ' ');
        std::istringstream iss(line);
        std::string key, value;
        if (iss >> key >> value) params[trim(key)] = trim(value);
    }
    return params;
}