#include "utils.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <omp.h>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input_file.txt\n";
        return 1;
    }

    try {
        auto params = parse_input_file(argv[1]);

        // Extract parameters
        int n_steps = std::stoi(params["n_steps"]);
        double delta_s = std::stod(params["delta_s"]);
        double c = std::stod(params["c"]);
        double angle_rot = std::stod(params["angle_rot"]);
        std::string angle_unit = params.count("angle_unit") ? params["angle_unit"] : "degrees";
        if (angle_unit == "degrees") angle_rot = angle_rot * M_PI / 180.0;

        std::string output_dir = params["output_dir"];
        fs::create_directories(output_dir);

        // Set number of threads
        int num_threads = params.count("num_threads") ? std::stoi(params["num_threads"]) : omp_get_max_threads();
        omp_set_num_threads(num_threads);
        std::cout << "Using OpenMP with " << num_threads << " threads.\n";

        // Process each pair of files in parallel
        std::istringstream files_set1_stream(params["files_set1"]);
        std::istringstream files_set2_stream(params["files_set2"]);
        std::vector<std::string> files1, files2;
        std::string file;

        while (files_set1_stream >> file) files1.push_back(file);
        while (files_set2_stream >> file) files2.push_back(file);

        if (files1.size() != files2.size()) {
            throw std::runtime_error("The number of files in the two sets must be the same.");
        }

#pragma omp parallel for schedule(guided)
        for (size_t i = 0; i < files1.size(); ++i) {
            std::string file1 = files1[i];
            std::string file2 = files2[i];

            // Read input files
            std::vector<Point> points1, points2;
            int num_points1, num_points2;
            read_xyz(file1, points1, num_points1);
            read_xyz(file2, points2, num_points2);

            // Define translation direction and rotation axis
            Eigen::Vector3d translation_direction(1.0, 0.0, 0.0); // Example direction
            Eigen::Vector3d rotation_axis(0.0, 1.0, 0.0); // Example axis

            // Preallocate transformed points
            std::vector<Point> transformed_points = points2;

            for (int step = 1; step <= n_steps; ++step) {
                // Reset transformed points to original points2
                std::copy(points2.begin(), points2.end(), transformed_points.begin());

                // Apply transformations
                translate_rotate_points(transformed_points, translation_direction, step * delta_s,
                                        rotation_axis, step * angle_rot);

                // Check minimum distance condition
                if (!check_min_distance(points1, transformed_points, c)) {
                    std::cerr << "Warning: Minimum distance violated at step " << step
                              << " for pair " << i + 1 << ". Skipping this step.\n";
                    continue;
                }

                // Write the combined coordinates to a new xyz file
                std::vector<Point> combined_points;
                combined_points.reserve(points1.size() + transformed_points.size());
                combined_points.insert(combined_points.end(), points1.begin(), points1.end());
                combined_points.insert(combined_points.end(), transformed_points.begin(), transformed_points.end());

                std::string output_file = output_dir + "/pair_" + std::to_string(i + 1) +
                                          "_step_" + std::to_string(step) + ".xyz";
                write_xyz(output_file, combined_points);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}