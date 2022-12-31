#include <Eigen/Dense>
#include "SDL2/SDL.h"
#include "helper.h"

void clear(SDL_Renderer * renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void draw(SDL_Renderer * renderer, SDL_Rect * rect) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, rect);
    SDL_RenderPresent(renderer);
}

bool hit_circle(const Eigen::VectorXd &vray_orig, const Eigen::VectorXd &ray_dir, const Eigen::VectorXd &circle_cent, const double &circle_rad){
    Eigen::VectorXd ac(3);
    ac << vray_orig - circle_cent;

    auto a = ray_dir.dot(ray_dir);
    auto b = 2*ac.dot(ray_dir);
    auto c = ac.dot(ac) - circle_rad*circle_rad;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}