#pragma once

// size of window
extern float width, height;

// parameters of screen
extern float h, d, f;
extern int screenWidth, screenHeight;

// parameters of camera
extern Vector cameraPosition;
extern Vector pRef;
extern Vector UP;
extern Camera camera;

// whether using back face culling or not
extern bool with_culling;

// all objects
extern vector<Object> objects;

// Zbuffer
extern vector<vector<float>> zDepth;
extern vector<vector<Vector>> zBuffer;