#include<iostream>
#include <Eigen/Dense>
#include "SDL2/SDL.h"
#include "helper.h"


int main(int argc, char *argv[]) {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 3.0;
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
    Eigen::VectorXd origin(3);
    origin << 0.0, 0.0, 0.0;
    
    Eigen::VectorXd focal_length(3);
    focal_length << 0.0, 0.0, 1.0;

    Eigen::VectorXd horizontal(3);
    horizontal << viewport_width, 0.0, 0.0;

    Eigen::VectorXd vertical(3);
    vertical << 0.0, viewport_height, 0.0;

    Eigen::VectorXd lower_left_corner(3);
    lower_left_corner << origin - horizontal/2 - vertical/2 - focal_length;

    // Let's define a circle given its center and radius
    const auto circle_radius = 0.5;

    Eigen::VectorXd circle_center(3);
    circle_center << 0, 0, 1;

    // Let's create the scene
    // Counter
    auto cont = 0;

    for (int j = image_height-1; j >= 0; --j){
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            Eigen::VectorXd ray_origin(3);
            ray_origin << 0.0, 0.0, 0.0;

            Eigen::VectorXd ray_direction(3);
            ray_direction << lower_left_corner + u*horizontal + v*vertical - origin;
            
            if (hit_circle(ray_origin, ray_direction, circle_center, circle_radius)){
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderDrawPoint(renderer, i, j);
            }

            cont += 1;

            if (cont == image_width) {
                cont = cont - image_width;
            }
        }
    }
    std::cout << "]\n";

        SDL_Rect rectangle = {
       .x = 10,
       .y = 10,
       .w = 10,
       .h = 10
    };



    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                // Handle arrow keys
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        rectangle.x -= 10;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        rectangle.x +=10;
                        break;
                    case SDL_SCANCODE_UP:
                        rectangle.y -= 10;
                        break;
                    case SDL_SCANCODE_DOWN:
                        rectangle.y += 10;
                        break;
                    default:
                        break;
                }

            } else if (event.type == SDL_QUIT) {
                quit = 1;
                puts("QUIT!");
                break;
            }
        }
    
    //clear(renderer); //Uncomment to see the moving rectangle
    draw(renderer, &rectangle);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}