#ifndef RAY_TRACING_H // include guard
#define RAY_TRACING_H

#include <Eigen/Dense>
#include "SDL2/SDL.h"


// IDEA: create an abstract GeometricBody class from which specific instances like Circle can subclass
class Circle {
    public:
        const double _radius;
        Eigen::Vector3d _center;
        Eigen::Vector3d _color;
    
        Circle(const double circle_radius, Eigen::Vector3d circle_center, Eigen::Vector3d circle_color);

        bool hit_circle(const Eigen::Vector3d &vray_orig, const Eigen::Vector3d &ray_dir, const Eigen::Vector3d &circle_cent, const double &circle_rad);

        const double get_radius() const;

        Eigen::Vector3d get_center() const;

        double get_color_R() const;

        double get_color_G() const;

        double get_color_B() const;

};


Eigen::Vector3d background_color(Eigen::Vector3d& ray);
void create_scene(Circle &circle, const int &image_width, const int&image_height, const Eigen::Vector3d &ray_origin, SDL_Renderer *renderer);
#endif
