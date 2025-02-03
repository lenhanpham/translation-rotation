#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>

struct Point {
    std::string name;
    double x, y, z;
};

// Function prototypes
void read_xyz(const std::string& file_path, std::vector<Point>& points, int& num_points);
void write_xyz(const std::string& file_path, const std::vector<Point>& points);
double calculate_distance(const Point& p1, const Point& p2);
bool check_min_distance(const std::vector<Point>& points1, const std::vector<Point>& points2, double min_dist);
void translate_rotate_points(std::vector<Point>& points, const Eigen::Vector3d& direction, double delta_s,
                             const Eigen::Vector3d& axis, double angle);
std::unordered_map<std::string, std::string> parse_input_file(const std::string& input_file);

#endif // UTILS_H