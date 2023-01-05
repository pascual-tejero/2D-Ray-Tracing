#include <Eigen/Dense> // Easier for vector multiplication
#include "SDL2/SDL.h"
#include "helper.h"


int main(int argc, char *argv[]) {

    // Image
    const auto aspect_ratio = 1.0;
    const unsigned int image_width = 800;
    const unsigned int image_height = static_cast<int>(image_width / aspect_ratio);

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

    // Let's define a circle given its center, radius and color
    Eigen::Vector3d circle_center(0.0, 0.0, 1.0);
    Eigen::Vector3d color(250, 118, 112);
    Circle circle(0.1, circle_center, color);

    // Let's create the scene
    create_scene(circle, image_width, image_height, renderer);
    
    
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) { 
                // Handle arrow keys
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        circle._center(0) += 0.1;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        circle._center(0) -= 0.1;
                        break;
                    case SDL_SCANCODE_UP:
                        circle._center(1) += 0.1;
                        break;
                    case SDL_SCANCODE_DOWN:
                        circle._center(1) -= 0.1;
                        break;
                    default:
                        break;
                }
                // Update circle position
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);

                create_scene(circle, image_width, image_height, renderer);

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