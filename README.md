Advanced Programming Project - Sprint 1

Files included in the repository: 
run.cpp, helper.cpp, helper.h and run (executable)

To execute the project, run in terminal:
g++ -I (Address of Eigen package) run.cpp helper.cpp -o run

Description:
We implemented all mentioned steps for completing the Sprint 1. We only defined a single circle as a first prototype, and we created a small screen of 10 pixels. In order to estimate the distance from the circle to the camera, we first evaluated the intersection of the ray with the circle, and if such intersection exists we computed the distance between the point of intersection and the camera.
(No copied repository into repository)
