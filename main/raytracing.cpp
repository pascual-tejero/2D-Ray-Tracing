#include <Eigen/Dense> // Easier for vector multiplication
#include "SDL2/SDL.h"
#include "helper.h"

// IDEA: to change the FOV we could change the focal distance and/or camera origin. 
// IDEA: Add another 2D geometric bodies with math equations in 2D to the scene.

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

    // Initialize the circle and camera positions
    Eigen::Vector3d circle_center(0.0, 0.0, 1.0);
    Eigen::Vector3d color(250, 118, 112);
    Circle circle(0.1, circle_center, color);

    Eigen::Vector3d ray_origin(0.0, 0.0, 0.0);

    create_scene(circle, image_width, image_height, ray_origin, renderer);
    SDL_RenderPresent(renderer);

    // Set the desired frame rate (e.g. 60 FPS)
    const int frame_rate = 60;
    const int frame_time_ms = 1000 / frame_rate;

    // Game loop
    int quit = 0;
    while (!quit) {
        // Handle user input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) { 
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
            }

            else if (event.type == SDL_MOUSEWHEEL){
                if(event.wheel.y > 0){  // scroll up
                    ray_origin(2) += 0.1;
                }

                else{  // scroll down
                    ray_origin(2) -= 0.1;
                }  
            }

            else if (event.type == SDL_QUIT) {
                quit = 1;
                puts("QUIT!");
                break;
            }
        }
        // update the position of the circle and/or camera, depending on the game logic
        circle_center(0) += 0.01;

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Re-create the scene and render it to the window
        create_scene(circle, image_width, image_height, ray_origin, renderer);
        SDL_RenderPresent(renderer);

        // Limit the frame rate by waiting for the desired time
        SDL_Delay(frame_time_ms);
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}






