#include<iostream>
#include <Eigen/Dense> // Easier  matrix and vector multiplications
#include "SDL2/SDL.h"
#include "helper.h"


int main(int argc, char *argv[]) {

    // Image
    const auto aspect_ratio = 1.0;
    const unsigned int image_width = 800;
    const unsigned int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;

    // Create SDL window and SDL renderer
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Raytracing Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image_width, image_height, SDL_WINDOW_OPENGL);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Define some objects for the scene
    Eigen::Vector3d origin(0.0, 0.0, 0.0);
    Eigen::Vector3d ray_origin(0.0, 0.0, 0.0);
    Eigen::Vector3d focal_length(0.0, 0.0, 1.0);
    Eigen::Vector3d horizontal(viewport_width, 0.0, 0.0);
    Eigen::Vector3d vertical(0.0, viewport_height, 0.0);
    Eigen::Vector3d lower_left_corner(origin - horizontal/2 - vertical/2 - focal_length);

    // Let's define a circle given its center, radius and color
    Eigen::Vector3d circle_center(0.0, 0.0, 1.0);
    Eigen::Vector3d color(250, 118, 112);
    Circle circle(0.1, circle_center, color);

    // Let's create the scene
    for (int j = image_height-1; j >= 0; --j){
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            Eigen::Vector3d ray_direction(lower_left_corner + u*horizontal + v*vertical - origin);
            
            if (circle.hit_circle(ray_origin, ray_direction, circle.get_center(), circle.get_radius())){
                SDL_SetRenderDrawColor(renderer, circle.get_color_R(), circle.get_color_G(), circle.get_color_B(), 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
    
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) { 
                // Handle arrow keys
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        ray_origin(0) -= 0.1;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        ray_origin(0) += 0.1;
                        break;
                    case SDL_SCANCODE_UP:
                        ray_origin(1) -= 0.1;
                        break;
                    case SDL_SCANCODE_DOWN:
                        ray_origin(1) += 0.1;
                        break;
                    default:
                        break;
                }
                // Update circle position
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);

                for (int j = image_height-1; j >= 0; --j){
                    for (int i = 0; i < image_width; ++i) {
                        auto u = double(i) / (image_width-1);
                        auto v = double(j) / (image_height-1);

                        Eigen::Vector3d ray_direction(lower_left_corner + u*horizontal + v*vertical - origin);
                        
                        if (circle.hit_circle(ray_origin, ray_direction, circle.get_center(), circle.get_radius())){
                            SDL_SetRenderDrawColor(renderer, circle.get_color_R(), circle.get_color_G(), circle.get_color_B(), 255);
                            SDL_RenderDrawPoint(renderer, i, j);
                        } 

                    }
                }

            } else if (event.type == SDL_QUIT) {
                quit = 1;
                puts("QUIT!");
                break;
            }
        }
    
    SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}