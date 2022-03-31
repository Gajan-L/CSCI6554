#pragma once
#include<unordered_set>
#include"Vector.h"
class PolygonType;

class Vertex {
private:
	Vector position;
	Vector posiAfterTransform;
	Vector normal;
	Vector color;
	unordered_set<int> adjacentPolygon;
public:
	Vertex();
	Vertex(float x, float y, float z);
	~Vertex();
	// set position
	void setPosition(Vector posi);
	// get position
	Vector getPosition();
	// set position after transform
	void setTransformPosition(Vector tposi);
	// get position after transform
	Vector getPositionAfterTransform();
	// update adjacent polygon
	void updateAdjPolygon(int p);
	// get adjacent polygon
	unordered_set<int> getAdjPolygon();
	// set normal
	void setNormal(Vector n);
	// get normal
	Vector getNormal();
	// set color
	void setColor(Vector c);
	// get color
	Vector getColor();
};