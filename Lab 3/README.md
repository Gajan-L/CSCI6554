# Shading and Illumination Modeling

Features:
 - Constant, Gouraud, Phong shading models;
 - Phong specular illumination model;
 - Support point light sources;
 - Fix round-up mistakes of previous version;
 - Rewrite some formats and classes (again...).

Parameters:
 - input file "better-ball.d.txt"
 - screen: h = 10.0, d = 25.0; f = 200.0;
 - pixel: 2000*2000;
 - camera: cameraPosition = (0, 0, 5); pRef = (0, 0, 0), UP = (0, 1, 0);
 - illumination: ka = 0.4; kd = 0.5; ks = 0.7; focusingFactor(n) = 24; light source  = (100, 100, 100); light intensity = （0.6, 0.6, 0.6); ambient intensity = (0, 0.25, 1.0).

Result:
 - Constant Shading:

![constant](https://user-images.githubusercontent.com/73145375/158914349-b11b700d-3995-40b5-b5b0-b1bee83ac0cc.png)
 - Gouraud Shading:

![gouraud](https://user-images.githubusercontent.com/73145375/158914403-9042583c-4f4c-4857-bea4-92d2887696fa.png)

 - Phong Shading:

![phong](https://user-images.githubusercontent.com/73145375/158914415-609f1113-a11f-4dbf-b6aa-5045a65b7c16.png)


p.s.
 - dont ever copy and paste the similar codes (even worse: they have similar viriable' names) you just wrote !!! it took me more than 3 hours to locate this bug (because they look very similar to the correct code)!!!
