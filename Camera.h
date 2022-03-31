#pragma once
#include"Matrix.h"
#include"Vector.h"

class Camera {
private:
	Matrix view;
	Vector position, N, U, V;
public:
	// default construction
	Camera();
	// construct a camera based on given parameters
	Camera(Vector pref, Vector c, Vector up);
	// destuctor
	~Camera();
	// initialize the view matrix
	void initView();
	// get the view matrix
	Matrix getView();
};