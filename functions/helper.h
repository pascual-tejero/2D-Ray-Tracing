#ifndef RAY_TRACING_H // include guard
#define RAY_TRACING_H

#include <string>
#include <memory>
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "SDL2/SDL.h"


// IDEA: create an abstract GeometricBody class from which specific instances like Circle can subclass
enum class GeometricBodyType{
    circle,
    square,
    triangle

};

class GeometricBody {
    protected:
        Eigen::Vector3d _center;
        const Eigen::Vector3d _color;
        const GeometricBodyType _type;

    public:
        GeometricBody(const Eigen::Vector3d&, const Eigen::Vector3d&, const GeometricBodyType&);

        virtual bool hit(const Eigen::Vector3d &, const Eigen::Vector3d &) const = 0;

        const Eigen::Vector3d get_center() const;

        const Eigen::Vector3d get_color() const;

        virtual ~GeometricBody();

        
};

class Circle : public GeometricBody {
    private:
        const double _radius;
        
    public:
        Circle(const double &, const Eigen::Vector3d &, const Eigen::Vector3d &, const GeometricBodyType &);

        bool hit(const Eigen::Vector3d &, const Eigen::Vector3d &) const override;

        const double get_radius() const;

        // ~Circle();

};

class Square : public GeometricBody {
    private:
        const double _side; 

    public:
        Square(const double &, const Eigen::Vector3d &, const Eigen::Vector3d &, const GeometricBodyType &);

        bool hit(const Eigen::Vector3d &, const Eigen::Vector3d &) const override;

        const double get_side() const;

        // ~Square();

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
        // std::vector<std::unique_ptr<GeometricBody>> objects;
        std::vector<GeometricBody*> objects;
        SDL_Renderer* _renderer;

        Scene(const double &, const unsigned int &, const unsigned int &, const double &, const double &, const Eigen::Vector3d &, 
            const Eigen::Vector3d &, SDL_Renderer* r);

        void move_camera_x(const double &);
        void move_camera_y(const double &);
        void move_camera_z(const double &);

        const Eigen::Vector3d background_color(const Eigen::Vector3d &) const;

        void create() const;

        ~Scene();
};


// TODO: declare as inline if function call is overhead
void Logger(const std::vector<double> &duration_str, const std::string &max_fps);

#endif
