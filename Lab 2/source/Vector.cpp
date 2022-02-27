#include"pch.h"

// default construction
Vector::Vector() {
	x = 0; y = 0; z = 0;
}

// construct by given parameters of x,y,z
Vector::Vector(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::~Vector() {}

// perform dot production
float Vector::dotProduct(Vector v) {
	return x * v.x + y * v.y + z * v.z;
}

// perform cross production
Vector Vector::crossProduct(Vector v) {
	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

// calculate distance of two vertices
float Vector::distance(Vector v) {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

// calculate magnitude of Vector
float Vector::magnitude() {
	return sqrtf(x * x + y * y + z * z);
}

// perform normalization of Vector
void Vector::normalize() {
	float tmp = 1.0f / magnitude();
	scalarMul(tmp);
}

// perform scalar multipilication
void Vector::scalarMul(float a) {
	x *= a;
	y *= a;
	z *= a;
}

// perform addition
Vector Vector::add(Vector v) {
	return Vector(x + v.x, y + v.y, z + v.z);
}

// perform subtraction
Vector Vector::sub(Vector v) {
	return Vector(x - v.x, y - v.y, z - v.z);
}
