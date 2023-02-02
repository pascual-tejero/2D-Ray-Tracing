#include <string>
#include <fstream>
#include <chrono>
#include <vector>
#include <Eigen/Dense> // Easier for matrix operations
#include "SDL2/SDL.h"
#include "helper.h"


int main(int argc, char *argv[]) {

    // Image
    const auto aspect_ratio = 1.0;
    const unsigned int image_width = 400;
    const unsigned int image_height = static_cast<int>(image_width / aspect_ratio);
    const auto viewport_height = 2.0;
    const auto viewport_width = aspect_ratio * viewport_height;

    const Eigen::Vector3d camera_origin(0.0, 0.0, 0.0);
    const Eigen::Vector3d focal_length(0.0, 0.0, 1.0);

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

    // Initialize the circles
    Eigen::Vector3d circle1_center(0.0, 0.0, 1.0);
    const Eigen::Vector3d circle1_color(250, 118, 112);

    Eigen::Vector3d circle2_center(0.5, 0.5, 1.0);
    const Eigen::Vector3d circle2_color(255, 128, 0); 

    Circle circle1(0.1, circle1_center, circle1_color, GeometricBodyType::circle);
    Circle circle2(0.2, circle2_center, circle2_color, GeometricBodyType::circle);    

    std::array<Circle, 2> circles_scene = {circle1, circle2};

    // Initialize the squares
    Eigen::Vector3d square1_center(-0.5, 0.5, 1.0);
    const Eigen::Vector3d square1_color(0, 128, 0);

    Square square1(0.4, square1_center, square1_color, GeometricBodyType::square);

    std::array<Square, 1> squares_scene = {square1};

    // Create scene
    Scene scene(aspect_ratio, image_width, image_height, viewport_height, viewport_width, camera_origin, focal_length, circles_scene, squares_scene, renderer);

    scene.create();
    SDL_RenderPresent(renderer);

    // Set the desired frame rate (e.g. 60 FPS)
    const int frame_rate = 60;
    const int frame_time_ms = 1000 / frame_rate;

    std::vector<double> time_per_frame;
    std::vector<std::string> duration_str;

    // Game loop
    int quit = 0;
    while (!quit) {
        // Start chrono 
        auto start = std::chrono::system_clock::now();
        // Handle user input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) { 
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        scene.move_camera_x(-0.05);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        scene.move_camera_x(0.05);
                        break;
                    case SDL_SCANCODE_UP:
                        scene.move_camera_y(-0.05);
                        break;
                    case SDL_SCANCODE_DOWN:
                        scene.move_camera_y(0.05);
                        break;
                    default:
                        break;
                }

                if (event.key.keysym.sym == SDLK_RETURN) { // press enter
                    quit = 1;
                    puts("QUIT!");
                    break;
                }
            }

            else if (event.type == SDL_MOUSEWHEEL){
                if(event.wheel.y > 0){  // scroll up
                    scene.move_camera_z(0.05);
                } 
                else{
                    scene.move_camera_z(-0.05);
                }  
            }

            else if (event.type == SDL_QUIT) {
                quit = 1;
                puts("QUIT!");
                break;
            }
        }
        

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Re-create the scene and render it to the window
        scene.create();
        SDL_RenderPresent(renderer);

        // Stop the chrono 
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // duration_str.push_back(std::to_string(duration.count()));
        time_per_frame.push_back(duration.count());

        // Limit the frame rate by waiting for the desired time
        SDL_Delay(frame_time_ms);
    }

    // Get maximum framerate
    const auto max_time = *std::max_element(time_per_frame.begin(), time_per_frame.end());
    const auto max_fps = 1.0/(max_time*0.001);  // conversion to seconds

    //Log maximum framerate 
    // Logger(duration_str, std::to_string(max_fps));
    Logger(time_per_frame, std::to_string(max_fps));
    
    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}






