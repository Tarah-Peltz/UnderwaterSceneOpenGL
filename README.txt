Controls:


w,a,s,d for forward, backward, left, and right movement
q,z for up and down movement
Arrow keys to adjust viewing angle


To Run:


Type make
Type ./FinalProject.exe


About the Project:


So far in the scene, I have implemented a skybox, created an oyster, implemented the two pass technique to create caustics, 
implemented fog (which is commented out to make things easier to see), created a bubble generation system, converted the 
texture loader into an RGBA texture loader to create transparent effects, and implemented the flag waving algorithm.

I want to import a few .obj files with shells and coral based on your .obj example.

I am in the process of implementing a collada file format loader to integrate skeletally rigged fish into the scene. 
I am following this highly detailed two part tutorial (http://www.wazim.com/Collada_Tutorial_1.htm). 

I am likely going to swap out the flag waving effect on the seaweed for something more like this tutorial 
(http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=32). The flag wave just doesn’t look right. 

I still have to fix the fog bug. I’ve isolated a few error points in my code, but I’m not sure how they’re affecting 
the results of fog. I managed to fix the bug in my control system. To get position, I was multiplying velocity by 
elapsed time instead of delta time. D’oh!

If you want, I can still create the collider system I originally promised to make the fish have conversations. 
However, I could also just focus on tackling other issues instead, like the collada loader and fine tuning the 
scenery, if that’s ok with you. 


