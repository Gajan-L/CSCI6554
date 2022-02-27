#pragma once
#include"Camera.h"
#include"Vector.h"
#include"Vertex.h"
#include"Polygon.h"
#include<string>
using namespace std;

// size of window
extern float width, height;

// parameters of screen
extern float h, d, f;

// parameters of camera
extern Vector camera_position;
extern Vector view_direction;
extern Vector UP;
extern Camera camera;

// whether using back face culling or not
extern bool with_culling;

// input file
extern string file;

// set of all vertices
extern Vertex vertices;

// set of all polygons
extern Polygons polygons;

// number of vertices and polygons
extern int v_num;
extern int p_num;
