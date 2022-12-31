#ifndef RAY_TRACING_H // include guard
#define RAY_TRACING_H

#include <Eigen/Dense>
#include "SDL2/SDL.h"


bool hit_circle(const Eigen::VectorXd &vray_orig, const Eigen::VectorXd &ray_dir, const Eigen::VectorXd &circle_cent, const double &circle_rad);

void clear(SDL_Renderer * renderer);

void draw(SDL_Renderer * renderer, SDL_Rect * rect);

#endif
