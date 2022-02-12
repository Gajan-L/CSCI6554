#include"GlobalVariable.h"

bool with_culling = true;

float width = 600.0;
float height = 600.0;

float h = 10.0;
float d = 10.0;
float f = 40.0;

Vector camera_position = Vector(0, 2, -5);
Vector view_direction = Vector(0, 0, 0);
Vector UP = Vector(0, 1, 0);
Camera camera;

string file = "D files/atc.d.txt";

int v_num;
int p_num;

Vertex vertices;
Polygons polygons;