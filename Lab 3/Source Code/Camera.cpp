#include"pch.h"
#include"Vector.h"
#include"Matrix.h"
#include"Camera.h"
#include"GlobalVariable.h"

// default construct a camera based on global parameters
Camera::Camera() {
	position = cameraPosition;
	N = pRef.sub(cameraPosition);
	N.normalize();
	Vector up = UP;
	up.normalize();
	U = N.crossProduct(up);
	U.normalize();
	V = U.crossProduct(N);
	view = Matrix();
	initView();
}

// construct a camera based on given parameters
Camera::Camera(Vector pref, Vector c, Vector up) {
	position = c;
	N = pref.sub(c);
	N.normalize();
	up.normalize();
	U = N.crossProduct(up);
	U.normalize();
	V = U.crossProduct(N);
	view = Matrix();
	initView();
}

// destuctor
Camera::~Camera() {
	view.~Matrix();
	position.~Vector();
	N.~Vector();
	V.~Vector();
	U.~Vector();
}

// set the view matrix
void Camera::initView() {
	// rotation matrix
	Matrix R = Matrix(4);
	// translate matrix
	Matrix T = Matrix(4);
	R.modify(0, 0, U.x);
	R.modify(0, 1, U.y);
	R.modify(0, 2, U.z);
	R.modify(1, 0, V.x);
	R.modify(1, 1, V.y);
	R.modify(1, 2, V.z);
	R.modify(2, 0, N.x);
	R.modify(2, 1, N.y);
	R.modify(2, 2, N.z);
	T.modify(0, 3, -position.x);
	T.modify(1, 3, -position.y);
	T.modify(2, 3, -position.z);

	view = R.mul(T);
}


Matrix Camera::getView() {
	return view;
}
