#pragma once

class Vector {
public:
	float x, y, z;
	// default construction
	Vector();
	// construct by given parameters of x,y,z
	Vector(float x, float y, float z);
	~Vector();
	// perform dot production
	float dotProduct(Vector v);
	// perform cross production
	Vector crossProduct(Vector v);
	// calculate distance of two vertices
	float distance(Vector v);
	// calculate magnitude of Vector
	float magnitude();
	// perform normalization of Vector
	void normalize();
	// perform scalar multipilication
	void scalarMul(float a);
	// perform subtraction
	Vector add(Vector v);
	Vector sub(Vector v);
	Vector mul(Vector v);
};