#pragma once
#include"Vector.h"
class Vertex;

struct Edge {
	int head;
	int tail;

	Edge() { head = 0, tail = 0; }
	Edge(int h, int t) : head(h), tail(t) {}
};

struct edgeTableType {
	int head;		// head node index
	int tail;		// tail node index
	int yMax;		// yMax
	float xMin, xoy;// xMin, delta x ober delta y
	edgeTableType(int head, int tail, int yMax, float xMin, float xoy) {
		this->head = head;
		this->tail = tail;
		this->yMax = yMax;
		this->xMin = xMin;
		this->xoy = xoy;
	}
};

struct PolygonType {
	int yMin, yMax;					// yMin and yMax of this polygon
	bool visible;					// whether this polygon is visible or not
	Vector ambient;					// RGB color
	vector<int> vertexOfPolygon;	// vertex of this polygon
	vector<Edge> edgeOfPolygon;		// edge of polygon
	Vector normal;					// normal of this polygon
	Vector center;					// certer point of polygon
};


class Object {
private:
	int vertexNum;												// num of vertices of object
	int polygonNum;												// num of polygons;
	vector<Vertex> vertexOfObject;								// vertices of object (1-indexed vector)
	//vector<Vector> vertexOfObject;								
	vector<PolygonType> polygonOfObject;						// polygons of object (0-indexed vector)
	vector<vector<pair<int, vector<edgeTableType>>>> edgeTable;	// edge table of each polygon; pair<yMin, edgeTableType>
	int shadingType;
public:
	// default construt an "empty" object
	Object();
	// construct an object given the set of vertices and polygons 
	Object(vector<Vertex> v, vector<PolygonType> p, int sType);
	// destructor
	~Object();
	// return size of vectex in this object
	int sizeOfVertex();
	// return size of polygon in this object
	int sizeOfPolygon();
	// get vertex by index
	Vertex getVertex(int i);
	// set vertex by index
	void setVertex(int i, Vertex v);
	// set vertex position given Vector posi
	void setVertexTransformPosi(int i, Vector posi);
	// get polygon by index
	PolygonType getPolygon(int i);
	// set polygon by index
	void setPolygon(int i, PolygonType p);
	// update yMin, yMax of polygon
	void updateYInfo();
	// update normal of vertex
	void updateVertexNormal();
	// randomly generate color
	Vector randomColoring();
	// create edge table for each polygon
	void createEdgeTable();
	// perform scan convertion
	void scanConvertion();
	// random shading
	void randomShading(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& edgeTable);
	// constant shading
	void constantShade(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& edgeTable);
	// gourand shading
	void gourandShade(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& edgeTable);
	// phong shading
	void phongShade(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& edgeTable);
};