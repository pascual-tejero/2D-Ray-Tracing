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

void create_scene(Circle &circle, const int &image_width, const int&image_height, const Eigen::Vector3d &ray_origin, SDL_Renderer *renderer){
    // Image
    const auto aspect_ratio = 1.0;

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;

    // Define some objects for the scene
    Eigen::Vector3d origin(0.0, 0.0, 0.0);
    Eigen::Vector3d focal_length(0.0, 0.0, 1.0);
    Eigen::Vector3d horizontal(viewport_width, 0.0, 0.0);
    Eigen::Vector3d vertical(0.0, viewport_height, 0.0);
    Eigen::Vector3d lower_left_corner(origin - horizontal/2 - vertical/2 - focal_length);

    for (int j = image_height-1; j >= 0; --j){
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            Eigen::Vector3d ray_direction(lower_left_corner + u*horizontal + v*vertical - origin);
            
            if (circle.hit_circle(ray_origin, ray_direction, circle.get_center(), circle.get_radius())){
                SDL_SetRenderDrawColor(renderer, circle.get_color_R(), circle.get_color_G(), circle.get_color_B(), 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
}

