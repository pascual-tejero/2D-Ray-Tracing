#include <fstream>
#include <Eigen/Dense>
#include "SDL2/SDL.h"
#include "helper.h"


Circle::Circle(const double circle_radius, Eigen::Vector3d circle_center, Eigen::Vector3d circle_color):
        _radius(circle_radius), _center(circle_center), _color(circle_color) {}


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

Eigen::Vector3d Circle::get_color() const {
    return _color;
}

Scene::Scene(double aspect_ratio, unsigned int image_width, unsigned int image_height, double viewport_height, double viewport_width, 
             Eigen::Vector3d camera_origin, Eigen::Vector3d focal_length, Circle& c, SDL_Renderer* r): 

             _aspect_ratio(aspect_ratio), _image_width(image_width), _image_height(image_height), _viewport_height(viewport_height), _viewport_width(viewport_width),
             _camera_origin(camera_origin), _focal_length(focal_length), circle(c), renderer(r)
{

    Eigen::Vector3d horizontal(_viewport_width, 0.0, 0.0);
    _horizontal = horizontal;

    Eigen::Vector3d vertical(0.0, _viewport_height, 0.0);
    _vertical = vertical;

    Eigen::Vector3d lower_left_corner(_camera_origin - _horizontal/2 - _vertical/2 - _focal_length);
    _lower_left_corner = lower_left_corner;

}

void Scene::move_camera_x(double increment){_camera_origin(0) += increment;}

void Scene::move_camera_y(double increment){_camera_origin(1) += increment;}

void Scene::move_camera_z(double increment){_camera_origin(2) += increment;}

Eigen::Vector3d Scene::background_color(Eigen::Vector3d& ray_direction){
    Eigen::Vector3d white_color(1.0, 1.0, 1.0);
    Eigen::Vector3d blue_color(0.5, 0.7, 1.0);
    Eigen::Vector3d unit_direction = ray_direction / ray_direction.norm();
    auto t = 0.5*(unit_direction(1) + 1.0);
    // Linear blending of white and blue depending on y coordinate of the ray direction
    return (1.0-t)*white_color + t*blue_color;
}

void Scene::create(){

    Eigen::Vector3d ray_origin(0.0, 0.0, 0.0);

    for (int j = _image_height-1; j >= 0; --j){
        for (int i = 0; i < _image_width; ++i) {
            auto u = double(i) / (_image_width-1);
            auto v = double(j) / (_image_height-1);

            Eigen::Vector3d ray_direction(_lower_left_corner + u*_horizontal + v*_vertical - ray_origin);
            
            if (circle.hit_circle(_camera_origin, ray_direction, circle.get_center(), circle.get_radius())){
                Eigen::Vector3d circle_color = circle.get_color();
                SDL_SetRenderDrawColor(renderer, circle_color(0), circle_color(1), circle_color(2), 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
            else{
                Eigen::Vector3d background = background_color(ray_direction);
                int r = background(0)*255;
                int g = background(1)*255;
                int b = background(2)*255;
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
}

// TODO: save all the output in a buffer object and only write it in the .log file at the very end. 
// Therefore, we don't open and close the file multiple times. 
void Logger(std::string msg, std::string frame){
    const std::string filepath = "time_stamp.log";  // run code from base directory
    // c_str() might not be necessary since C++11
    std::ofstream ofs(filepath.c_str(), std::ios_base::out | std::ios_base::app);
    if (frame == "Maximum FPS"){
        ofs << frame << ":" << "\t" << msg << " fps" <<'\n';
    }
    else{
        ofs << "Frame " << frame << ":" << "\t" << msg << " ms" <<'\n';
    }
    
    ofs.close();
}


