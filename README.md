# Project: 2D Ray Tracing #

The following project performed as a part of "Advanced Programming" course offered by Technical University of Munich (TUM) in the winter semester of 2022/2023. 

## Project Members ##
1. Balle, Marc
2. Tejero, Pascual

## Description ##
We defined a 2D screen and created a small circle. In order to estimate the distance from the circle to the camera, we first evaluated the intersection of the ray with the circle. If such intersection exists, we computed the distance between the point of intersection and the camera, and a red dot is created, otherwise a black dot.

Addtionallly, the user can adjust the position of the origin of the rays with the arrow keys of the keyboard in such a way that the circle can be moved in four directions. By using the scroll on the mouse, the user can zoom-in or zoom-out the scene. 

![alt-text-1](img/result1.png "Position of the circle in the center") 
![alt-text-2](img/result2.png "Position of the circle on one side ")

## Run the code ##
In the folder where you cloned the repository, run the following in the terminal:  
`cmake CMakeLists.txt`  
`cd main`  
`make`  
`./project_raytracing`  

## References ##
[1] Shirley, P., Ray Tracing in one weekend series. Edited by S. Hollasch and T.D. Black. Available at https://raytracing.github.io/books/RayTracingInOneWeekend.html  
[2] Eigen. Available at: https://eigen.tuxfamily.org/dox/  
[3] SDL2/Frontpage - SDL Wiki. Available at: https://wiki.libsdl.org/SDL2/FrontPage  
 
