#include"pch.h"
#include"Vector.h"
#include"Vertex.h"


Vertex::Vertex() {
	position = Vector();
	normal = Vector();
	color = Vector();
	adjacentPolygon.clear();
}
Vertex::Vertex(float x, float y, float z) {
	position = Vector(x, y, z);
}
Vertex::~Vertex() {
	position.~Vector();
	normal.~Vector();
	color.~Vector();
	adjacentPolygon.clear();

}
// set position
void Vertex::setPosition(Vector posi) {
	position = posi;
}
// get position
Vector Vertex::getPosition() {
	return position;
}
// set position after transform
void Vertex::setTransformPosition(Vector tposi) {
	posiAfterTransform = tposi;
}
// get position after transform
Vector Vertex::getPositionAfterTransform() {
	return posiAfterTransform;
}

// update adjacent polygon
void Vertex::updateAdjPolygon(int p) {
	adjacentPolygon.insert(p);
}

// get adjacent polygon
unordered_set<int> Vertex::getAdjPolygon() {
	return adjacentPolygon;
}

// set normal
void Vertex::setNormal(Vector n) {
	normal = n;
}
// get normal
Vector Vertex::getNormal() {
	return normal;
}
// set color
void Vertex::setColor(Vector c) {
	color = c;
}
// get color
Vector Vertex::getColor() {
	return color;
}