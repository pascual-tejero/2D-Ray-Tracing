#ifndef RAY_TRACING_H // include guard
#define RAY_TRACING_H

#include <string>
#include <Eigen/Dense>
#include "SDL2/SDL.h"


// IDEA: create an abstract GeometricBody class from which specific instances like Circle can subclass
enum class GeometricBodyType{
    circle,
    square,
    triangle

};

class GeometricBody {
    public:
        virtual bool hit(const Eigen::Vector3d &, const Eigen::Vector3d &) const = 0;

        
};

class Circle : public GeometricBody {
    private:
        const double _radius;
        Eigen::Vector3d _center;
        const Eigen::Vector3d _color;
        const GeometricBodyType _type;
        
    public:
        Circle(const double &, const Eigen::Vector3d &, const Eigen::Vector3d &, const GeometricBodyType &);

        bool hit(const Eigen::Vector3d &, const Eigen::Vector3d &) const override;

        const double get_radius() const;

        const Eigen::Vector3d get_center() const;

        const Eigen::Vector3d get_color() const;

};

class Square : public GeometricBody {
    private:
        const double _side;
        Eigen::Vector3d _center;
        const Eigen::Vector3d _color;
        const GeometricBodyType _type;  

    public:
        Square(const double &, const Eigen::Vector3d &, const Eigen::Vector3d &, const GeometricBodyType &);

        bool hit(const Eigen::Vector3d &, const Eigen::Vector3d &) const override;

        const double get_side() const;

        const Eigen::Vector3d get_center() const;

        const Eigen::Vector3d get_color() const;

};  


class Scene {
    private:
        // Image
        const double _aspect_ratio;
        const  unsigned int _image_width;
        const unsigned int _image_height;

        // Camera
        const double _viewport_height;
        const double _viewport_width;

        // Define some objects for the scene
        Eigen::Vector3d _camera_origin;
        const Eigen::Vector3d _focal_length;
        Eigen::Vector3d _horizontal;
        Eigen::Vector3d _vertical;
        Eigen::Vector3d _lower_left_corner;

    public:
        std::array<Circle, 2> _circles_scene;
        std::array<Square, 1> _squares_scene;
        SDL_Renderer* _renderer;

        Scene(const double &, const unsigned int &, const unsigned int &, const double &, const double &, const Eigen::Vector3d &, 
            const Eigen::Vector3d &, const std::array<Circle, 2> &, const std::array<Square, 1> &, SDL_Renderer* r);

        void move_camera_x(const double &);
        void move_camera_y(const double &);
        void move_camera_z(const double &);

        const Eigen::Vector3d background_color(const Eigen::Vector3d &) const;

        void create() const;
};


// TODO: declare as inline if function call is overhead
void Logger(const std::vector<double> &duration_str, const std::string &max_fps);

#endif
