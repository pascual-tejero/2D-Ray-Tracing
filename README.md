# Project: 2D Ray Tracing #

The following project performed as a part of "Advanced Programming" course offered by Technical University of Munich (TUM) in the winter semester of 2022/2023. 

## Project Members ##
1. Balle Sánchez, Marc
2. Tejero Cervera, Pascual

## Description ##
We have defined a 2D screen and created two circles and one square on it. To estimate the distance from the objects to the camera, we evaluate the intersection of the ray with the corresponding object. If an intersection exists, we compute the distance between the point of intersection and the camera and display a pixel of a specific color. If no intersection exists, the background color is displayed.

The user can adjust the origin of the rays using the arrow keys on the keyboard, allowing them to move the objects in four directions. The scene can be zoomed in or out using the mouse scroll wheel. The program terminates and closes the window when the user presses the Enter key.

Whenever the user interacts with the scene by moving or zooming the objects, the calculation time per frame is recorded. Upon program termination, a file named _time_stamp.log_ is created, which contains a list of the calculation time per frame at each moment and the maximum number of frames per second (FPS).

<table>
  <tr>
    <td>
      ![neutral_pos](images/general_layout.png "General layout")
    </td>
    <td>
      ![moved_pos](images/move_objects.png "Object moved")
    </td>
  </tr>
  <tr>
    <td>
      ![zoom_in](images/zoom_in.png "Zoom in")
    </td>
    <td>
      ![zoom_out](images/zoom_out.png "Zoom out")
    </td>
  </tr>
</table>

## Prerequisites ##
In order to compile and run the code, it is necessary to download the SDL2 and Eigen libraries into the system. One can simply do that by entering in the terminal:  
`sudo apt-get install libsdl2-dev`  
`sudo apt install libeigen3-dev`  

## Run the code ##
In the folder where you cloned the repository, run the following in the terminal:  
`cmake CMakeLists.txt`  
`cd main`  
`make`  
`./project_raytracing` 

## Code optimization ##
To optimize the code, we started by profiling the program. We discovered that the bottleneck was in the `void create() const` method of the _Scene_ class. This method is called at the start of the program or when the user moves or zooms the objects, and it updates all pixels in the window. 

To improve the efficiency and performance of the program, we took the following steps:
1. We declared all necessary variables as `const` to allow for execution at compile-time.
2. In the `void create() const` method, we used the `#pragma omp parallel for` directive from the OpenMP library to make the second for loop parallel.
3. We implemented runtime polymorphism using a base class _GeometricBody_ and derived classes _Circle_ and _Square_. We created objects of these classes using raw pointers and stored them in a vector of _GeometricBody_ objects, rather than having separate vectors for each class. This allowed us to avoid having two separate loops for each class and instead have a single loop that iterates through all _GeometricBody_ objects and updates their positions.

## References ##
[1] Shirley, P., Ray Tracing in one weekend series. Edited by S. Hollasch and T.D. Black. Available at: https://raytracing.github.io/books/RayTracingInOneWeekend.html  
[2] Eigen. Available at: https://eigen.tuxfamily.org/dox/  
[3] SDL2/Frontpage - SDL Wiki. Available at: https://wiki.libsdl.org/SDL2/FrontPage  
 
