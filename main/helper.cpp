#include <Eigen/Dense>
#include "SDL2/SDL.h"
#include "helper.h"


Circle::Circle(const double circle_radius, Eigen::Vector3d circle_center, Eigen::Vector3d circle_color):
        _radius(circle_radius) {
        _center = circle_center; 
        _color = circle_color;
    }


bool Circle::hit_circle(const Eigen::Vector3d &vray_orig, const Eigen::Vector3d &ray_dir, const Eigen::Vector3d &circle_cent, const double &circle_rad){
    Eigen::Vector3d ac(vray_orig - circle_cent);

    auto a = ray_dir.dot(ray_dir);
    auto b = 2*ac.dot(ray_dir);
    auto c = ac.dot(ac) - circle_rad*circle_rad;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

const double Circle::get_radius() const{
    return _radius;
}

Eigen::Vector3d Circle::get_center() const {
    return _center;
}

double Circle::get_color_R() const{
    return _color(0);
}

double Circle::get_color_G() const{
    return _color(1);
}

double Circle::get_color_B() const{
    return _color(2);
}



