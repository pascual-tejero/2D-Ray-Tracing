#ifndef RAY_TRACING_H // include guard
#define RAY_TRACING_H

#include<iostream>
#include<vector>
#include<math.h>
#include <Eigen/Dense>

bool hit_circle(const Eigen::VectorXd &vray_orig, const Eigen::VectorXd &ray_dir, const Eigen::VectorXd &circle_cent, const double &circle_rad);

#endif
