#include <fstream>
#include <filesystem>
#include <algorithm>
#include <execution>
#include <Eigen/Dense>
#include "SDL2/SDL.h"
#include "helper.h"

GeometricBody::GeometricBody(const Eigen::Vector3d& center, const Eigen::Vector3d& color, const GeometricBodyType& type):
    _center(center), _color(color), _type(type) {}

const Eigen::Vector3d GeometricBody::get_center() const {
    return _center;
}

const Eigen::Vector3d GeometricBody::get_color() const {
    return _color;
}

GeometricBody::~GeometricBody() {}

Circle::Circle(const double &radius, const Eigen::Vector3d &center, const Eigen::Vector3d &color, const GeometricBodyType &type):
        GeometricBody(center, color, type), _radius(radius) {};


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

Square::Square(const double &side, const Eigen::Vector3d &center, const Eigen::Vector3d &color, const GeometricBodyType &type):
        GeometricBody(center, color, type), _side(side) {};

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


Scene::Scene(const double &aspect_ratio, const unsigned int &image_width, const unsigned int &image_height, const double &viewport_height, 
             const double &viewport_width, const Eigen::Vector3d &camera_origin, const Eigen::Vector3d &focal_length, SDL_Renderer* r): 

             _aspect_ratio(aspect_ratio), _image_width(image_width), _image_height(image_height), _viewport_height(viewport_height), 
             _viewport_width(viewport_width), _camera_origin(camera_origin), _focal_length(focal_length), _renderer(r)
{

    const Eigen::Vector3d horizontal(_viewport_width, 0.0, 0.0);
    _horizontal = horizontal;

    const Eigen::Vector3d vertical(0.0, _viewport_height, 0.0);
    _vertical = vertical;

    const Eigen::Vector3d lower_left_corner(_camera_origin - _horizontal/2 - _vertical/2 - _focal_length);
    _lower_left_corner = lower_left_corner;

    // Initialize the circles
    const Eigen::Vector3d circle1_center(0.0, 0.0, 1.0);
    const Eigen::Vector3d circle1_color(250, 118, 112);

    const Eigen::Vector3d circle2_center(0.5, 0.5, 1.0);
    const Eigen::Vector3d circle2_color(255, 128, 0); 

    // Initialize the squares
    const Eigen::Vector3d square1_center(-0.5, 0.5, 1.0);
    const Eigen::Vector3d square1_color(0, 128, 0);

    // Add figures to the objects array that scene will actually use
    // std::unique_ptr<GeometricBody> circle1 = std::make_unique<Circle>(0.1, circle1_center, circle1_color, GeometricBodyType::circle);
    // std::unique_ptr<GeometricBody> circle2 = std::make_unique<Circle>(0.2, circle2_center, circle2_color, GeometricBodyType::circle);
    // std::unique_ptr<GeometricBody> square = std::make_unique<Square>(0.4, square1_center, square1_color, GeometricBodyType::square);

    GeometricBody* c1 = new Circle(0.1, circle1_center, circle1_color, GeometricBodyType::circle);
    GeometricBody* c2 = new Circle(0.2, circle2_center, circle2_color, GeometricBodyType::circle);
    GeometricBody* sq = new Square(0.4, square1_center, square1_color, GeometricBodyType::square);

    objects.push_back(c1);
    objects.push_back(c2);
    objects.push_back(sq);
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
        #pragma omp parallel for
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
            for (auto &object : objects) {
                if (object->hit(_camera_origin, ray_direction)){
                    // Makes no sense to have get_color() if attributes are protected. We could access them directly. Or we declare the attributes private.
                    Eigen::Vector3d color = object->get_color();
                    SDL_SetRenderDrawColor(_renderer, color(0), color(1), color(2), 255);
                    SDL_RenderDrawPoint(_renderer, i, j);
                }
            }
            /*
            std::for_each(std::execution::par, objects.begin(), objects.end(), [&](auto&& item){
                if (item->hit(_camera_origin, ray_direction)){
                    // Makes no sense to have get_color() if attributes are protected. We could access them directly. Or we declare the attributes private.
                    Eigen::Vector3d color = item->get_color();
                    SDL_SetRenderDrawColor(_renderer, color(0), color(1), color(2), 255);
                    SDL_RenderDrawPoint(_renderer, i, j);
                }
            });*/

        }
    }
}


Scene::~Scene(){
    for (auto& elem: objects){
        delete elem;
    }
}

// Save all the output in a buffer object and only write it in the .log file at the very end. 
void Logger(const std::vector<double> &duration_str, const std::string &max_fps){
    const std::string filepath = static_cast<std::string>(std::filesystem::current_path()) + "/../../time_stamp.log";  // run code from base directory
    int frame_number = 0;
    std::ofstream ofs(filepath.c_str(), std::ios_base::out | std::ios_base::trunc);
    for (auto duration : duration_str) {
        ofs << "Frame " << frame_number << ":" << "\t" << duration << " ms" <<'\n';
        frame_number++;
    }
    ofs << "Maximum FPS:" << "\t" << max_fps << " fps" <<'\n';
    
    ofs.close();
}


