#include"pch.h"

// size of window
float width = 800.0;
float height = 800.0;

// parameters of screen
float h = 10.0;
float d = 10.0;
float f = 40.0;
int screenWidth = 2000;
int screenHeight = 2000;

// parameters of camera
Vector cameraPosition = Vector(1, 2, 5);
Vector pRef = Vector(0, 0, 0);
Vector UP = Vector(0, 1, 0);
Camera camera;

// whether using back face culling or not
bool with_culling = true;

// all objects
vector<Object> objects;

// Zbuffer
vector<vector<float>> zDepth(screenWidth + 1, vector<float>(screenHeight + 1, 1.0f));
vector<vector<Vector>> zBuffer(screenWidth + 1, vector<Vector>(screenHeight + 1, Vector{ 0,0,0 }));