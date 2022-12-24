#include<iostream>
#include<vector>
#include<math.h>
#include <Eigen/Dense>
#include "helper.h"


int main(){

    // Screen
    const int n_pixels = 10;

    // Camera
    auto focal_length = 1.0;

    Eigen::VectorXd origin(2);
    origin << 0.0, 0.0;

    Eigen::VectorXd horizontal(2);
    horizontal << n_pixels, 0.0;

    Eigen::VectorXd left_side(2);
    left_side << -n_pixels/2.0, focal_length;

    // Render

    // First define some objects for the scene
    // Let's define a circle given its center and radius
    const auto circle_radius = 0.25;

    Eigen::VectorXd circle_center(2);
    circle_center << 1.0, 1.0;

    // Now let's create the scene
    std::cout<<"Rendering image...\n";
    std::cout<<"[";

    for (int i = 0; i < n_pixels; ++i){
        auto u = double(i) / n_pixels;

        Eigen::VectorXd ray_origin(2);
        ray_origin << 0.0, 0.0;

        Eigen::VectorXd ray_direction(2);
        ray_direction << left_side + u*horizontal - origin;

        if (hit_circle(ray_origin, ray_direction, circle_center, circle_radius)){
            std::cout << sqrt(circle_center.dot(circle_center)) << ", ";
        }
        else{
            std::cout << 255.0 << ", ";  // max pixel intensity in a 8-bits image (= 2⁸ - 1) 
        }
    }
    std::cout << "]\n";

}