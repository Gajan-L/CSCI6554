#pragma once
#include"Camera.h"
#include"Vector.h"

// preprocessing input date by removing all '\t'
string preprocess(string& s, const string& old, const string& replacement);

// split string by the split char
vector<string> split(const string& s, char p);

// load data from file
void loadData(string file);

// perspective matrix
Matrix perspective();

// local to screen transformation
Vector transformation(Vector v);

// perform the transformation to vertices of each object
void performTransformation();

// perform back face culling after transformation
void backFaceCulling();

// perform scan convertion
void performScanConvertion();