#include<iostream>
#include<vector>
#include<math.h>
#include "helper.h"

bool hit_circle(const Eigen::VectorXd &vray_orig, const Eigen::VectorXd &ray_dir, const Eigen::VectorXd &circle_cent, const double &circle_rad){
    Eigen::VectorXd ac(2);
    ac << ray_dir - circle_cent;

    auto a = ray_dir.dot(ray_dir);
    auto b = 2*ray_dir.dot(ac);
    auto c = ac.dot(ac) - circle_rad*circle_rad;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}