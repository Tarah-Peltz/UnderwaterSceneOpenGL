Controls:


w,a,s,d for forward, backward, left, and right movement
q,z for up and down movement
Arrow keys to adjust viewing angle


To Run:


Type make
Type ./FinalProject.exe


About the Project:

I implemented caustics by doing a two pass system. In the first pass, the objects are drawn normally, 
and in the second pass, the existing object textures are blended with the caustics. Since the crab and
fish are obj files, these do not have caustics (I didn't know how to do a two pass with a mtl file).

I also altered the texture reader provided to convert to an RGBA format and made black have an alpha of 0.
This allowed me to implement the bubbles, blue coral, and seaweed. I also had to write my own seaweed equation because
flag waving effects just didn't look right.

I also created my own coral tubes from two cylinders that taper inward and a disc on the bottom. There is
a little bug in the normals of the cylinder where it closes.

I wrote the code for an oyster as well with four half spheres and a complete sphere to look like the shell
and pearl.

I also downloaded two obj files and converted their textures to bmp. Then I edited the mtl files to accept bmp
instead of their old file formats. 

I also added fog and wrote a bubble generation system that allows you to specify the number of bubbles and randomly
creates them. Once a bubble hits the top of the skybox, it is moved back down to a y value of 0 so I'm not constantly
creating new bubbles.  

