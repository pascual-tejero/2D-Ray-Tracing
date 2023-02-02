#include <fstream>
#include <Eigen/Dense>
#include "SDL2/SDL.h"
#include "helper.h"


Circle::Circle(const double &radius, const Eigen::Vector3d &center, const Eigen::Vector3d &color, const GeometricBodyType &type):
        _radius(radius), _center(center), _color(color), _type(type) {};


bool Circle::hit(const Eigen::Vector3d &cam_orig, const Eigen::Vector3d &ray_dir) const{
    const Eigen::Vector3d ac(_center - cam_orig);

    const auto a = ray_dir.dot(ray_dir);
    const auto b = 2*ac.dot(ray_dir);
    const auto c = ac.dot(ac) - _radius*_radius;
    const auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

const double Circle::get_radius() const{
    return _radius;
}

const Eigen::Vector3d Circle::get_center() const {
    return _center;
}

const Eigen::Vector3d Circle::get_color() const {
    return _color;
}

Square::Square(const double &side, const Eigen::Vector3d &center, const Eigen::Vector3d &color, const GeometricBodyType &type):
        _side(side), _center(center), _color(color), _type(type) {};

bool Square::hit(const Eigen::Vector3d &cam_orig, const Eigen::Vector3d &ray_dir) const {
    const Eigen::Vector3d ac(cam_orig - ray_dir);

    // Compute the intersection point
    if (fabs(ac(0) - _center(0)) > _side / 2) return false;
    if (fabs(ac(1) - _center(1)) > _side / 2) return false;
    // if (fabs(ac(2) - _center(2)) > _side / 2) return false;
    return true;
}

const double Square::get_side() const{
    return _side;
}

const Eigen::Vector3d Square::get_center() const {
    return _center;
}

const Eigen::Vector3d Square::get_color() const {
    return _color;
}

Scene::Scene(const double &aspect_ratio, const unsigned int &image_width, const unsigned int &image_height, const double &viewport_height, 
             const double &viewport_width, const Eigen::Vector3d &camera_origin, const Eigen::Vector3d &focal_length, const std::array<Circle, 2> &c, 
             const std::array<Square, 1> &s, SDL_Renderer* r): 

             _aspect_ratio(aspect_ratio), _image_width(image_width), _image_height(image_height), _viewport_height(viewport_height), 
             _viewport_width(viewport_width), _camera_origin(camera_origin), _focal_length(focal_length), _circles_scene(c), _squares_scene(s), _renderer(r)
{

    const Eigen::Vector3d horizontal(_viewport_width, 0.0, 0.0);
    _horizontal = horizontal;

    const Eigen::Vector3d vertical(0.0, _viewport_height, 0.0);
    _vertical = vertical;

    const Eigen::Vector3d lower_left_corner(_camera_origin - _horizontal/2 - _vertical/2 - _focal_length);
    _lower_left_corner = lower_left_corner;

}

void Scene::move_camera_x(const double &increment){_camera_origin(0) += increment;}

void Scene::move_camera_y(const double &increment){_camera_origin(1) += increment;}

void Scene::move_camera_z(const double &increment){_camera_origin(2) += increment;}

const Eigen::Vector3d Scene::background_color(const Eigen::Vector3d &ray_direction) const {
    const Eigen::Vector3d white_color(1.0, 1.0, 1.0);
    const Eigen::Vector3d blue_color(0.5, 0.7, 1.0);
    const Eigen::Vector3d unit_direction = ray_direction / ray_direction.norm();
    const auto t = 0.5*(unit_direction(1) + 1.0);
    // Linear blending of white and blue depending on y coordinate of the ray direction
    return (1.0-t)*white_color + t*blue_color;
}

void Scene::create() const{

    for (int j = _image_height-1; j >= 0; --j){
        for (int i = 0; i < _image_width; ++i) {
            const auto u = double(i) / (_image_width-1);
            const auto v = double(j) / (_image_height-1);

            const Eigen::Vector3d ray_direction(_lower_left_corner + u*_horizontal + v*_vertical - _camera_origin);

            const Eigen::Vector3d background = background_color(ray_direction);

            const int r = background(0)*255;
            const int g = background(1)*255;
            const int b = background(2)*255;

            SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
            SDL_RenderDrawPoint(_renderer, i, j);        
            
            // Draw circles
            for (auto &circle : _circles_scene) {
                if (circle.hit(_camera_origin, ray_direction)){
                    Eigen::Vector3d circle_color = circle.get_color();
                    SDL_SetRenderDrawColor(_renderer, circle_color(0), circle_color(1), circle_color(2), 255);
                    SDL_RenderDrawPoint(_renderer, i, j);
                }
            }

            // Draw squares
            for (auto &square : _squares_scene) {
                if (square.hit(_camera_origin, ray_direction)){
                    Eigen::Vector3d square_color = square.get_color();
                    SDL_SetRenderDrawColor(_renderer, square_color(0), square_color(1), square_color(2), 255);
                    SDL_RenderDrawPoint(_renderer, i, j);
                }
            }
        }
    }
}

// Save all the output in a buffer object and only write it in the .log file at the very end. 
void Logger(const std::vector<double> &duration_str, const std::string &max_fps){
    const std::string filepath = "time_stamp.log";  // run code from base directory
    int frame_number = 0;
    std::ofstream ofs(filepath.c_str(), std::ios_base::out | std::ios_base::trunc);
    for (auto duration : duration_str) {
        ofs << "Frame " << frame_number << ":" << "\t" << duration << " ms" <<'\n';
        frame_number++;
    }
    ofs << "Maximum FPS:" << "\t" << max_fps << " fps" <<'\n';
    
    ofs.close();
}


