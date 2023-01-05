#ifndef RAY_TRACING_H // include guard
#define RAY_TRACING_H

#include <Eigen/Dense>
#include "SDL2/SDL.h"

void clear(SDL_Renderer * renderer);

void draw(SDL_Renderer * renderer, SDL_Rect * rect);

class Circle{
    private:
        const double _radius;
        Eigen::Vector3d _center;
        Eigen::Vector3d _color;
    public:
        Circle(const double circle_radius, Eigen::Vector3d circle_center, Eigen::Vector3d circle_color);

        bool hit_circle(const Eigen::Vector3d &vray_orig, const Eigen::Vector3d &ray_dir, const Eigen::Vector3d &circle_cent, const double &circle_rad);

        const double get_radius() const;

        Eigen::Vector3d get_center() const;

        double get_color_R() const;

        double get_color_G() const;

        double get_color_B() const;

};



#endif
