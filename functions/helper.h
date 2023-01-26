#ifndef RAY_TRACING_H // include guard
#define RAY_TRACING_H

#include <Eigen/Dense>
#include "SDL2/SDL.h"


// IDEA: create an abstract GeometricBody class from which specific instances like Circle can subclass
class Circle {
    private:
        const double _radius;
        Eigen::Vector3d _center;
        const Eigen::Vector3d _color;
    public:
        Circle(const double circle_radius, Eigen::Vector3d circle_center, Eigen::Vector3d circle_color);

        bool hit_circle(const Eigen::Vector3d &vray_orig, const Eigen::Vector3d &ray_dir, const Eigen::Vector3d &circle_cent, const double &circle_rad);

        const double get_radius() const;

        Eigen::Vector3d get_center() const;

        Eigen::Vector3d get_color() const;

};

class Scene {
    private:
        // Image
        const double _aspect_ratio;
        unsigned int _image_width;
        unsigned int _image_height;

        // Camera
        double _viewport_height;
        double _viewport_width;

        // Define some objects for the scene
        Eigen::Vector3d _camera_origin;
        Eigen::Vector3d _focal_length;
        Eigen::Vector3d _horizontal;
        Eigen::Vector3d _vertical;
        Eigen::Vector3d _lower_left_corner;
        Eigen::Vector3d _ray_origin;

    public:

        Circle& circle;
        SDL_Renderer* renderer;

        Scene(double, unsigned int, unsigned int, double, double, Eigen::Vector3d, Eigen::Vector3d, Circle&, SDL_Renderer*);

        void move_camera_x(double);
        void move_camera_y(double);
        void move_camera_z(double);


        Eigen::Vector3d background_color(Eigen::Vector3d&);

        void create();
};


Eigen::Vector3d background_color(Eigen::Vector3d& ray);
void create_scene(Circle &circle, const int &image_width, const int&image_height, const Eigen::Vector3d &ray_origin, SDL_Renderer *renderer);
#endif
