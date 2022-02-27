#pragma once
using namespace std;
class Vector {
private:
	float x, y, z;
public:
	// default construction
	Vector() {
		x = 0; y = 0; z = 0;
	}
	// construct by given parameters of x,y,z
	Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	// modify x,y,z of a Vector
	void set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	// perform dot production
	float dotProduct(Vector v) {
		return x * v.getX() + y * v.getY() + z * v.getZ();
	}
	// perform cross production
	Vector crossProduct(Vector v) {
		return Vector(y * v.getZ() - z * v.getY(), z * v.getX() - x * v.getZ(), x * v.getY() - y * v.getX());
	}
	// calculate distance of two vertices
	float distance(Vector v) {
		return sqrtf((x - v.getX()) * (x - v.getX()) + (y - v.getY()) * (y - v.getY()) + (z - v.getZ()) * (z - v.getZ()));
	}
	// calculate magnitude of Vector
	float magnitude() {
		return sqrtf(x * x + y * y + z * z);
	}
	// perform normalization of Vector
	void normalize() {
		float tmp = 1.0f / magnitude();
		scalarMul(tmp);
	}
	// perform scalar multipilication
	void scalarMul(float a) {
		x *= a;
		y *= a;
		z *= a;
	}
	// perform subtraction
	Vector sub(Vector v) {
		return Vector(x - v.getX(), y - v.getY(), z - v.getZ());
	}
	// get the x,y,z of Vector
	float getX() {
		return x;
	}
	float getY() {
		return y;
	}
	float getZ() {
		return z;
	}
};