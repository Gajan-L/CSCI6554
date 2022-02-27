#pragma once
struct Edge {
	int head;
	int tail;
	Edge() {
		head = 0;
		tail = 0;
	}
	Edge(int h, int t) {
		head = h;
		tail = t;
	}
};

struct edgeTableType {
	Vector head;	// head node
	Vector tail;	// tail node
	int yMax;		// yMax
	float xMin, xoy;// xMin, delta x ober delta y
	edgeTableType(Vector head, Vector tail, int yMax, float xMin, float xoy) {
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
	float colorR, colorG, colorB;	// RGB color
	vector<int> vertexOfPolygon;	// vertex of this polygon
	vector<Edge> edgeOfPolygon;		// edge of polygon
};

class Object {
private:
	int vertexNum;												// num of vertices of object
	int polygonNum;												// num of polygons;
	vector<Vector> vertexOfObject;								// vertices of object (1-indexed vector)
	vector<PolygonType> polygonOfObject;						// polygons of object (0-indexed vector)
	vector<vector<pair<int,vector<edgeTableType>>>> edgeTable;	// edge table of each polygon; pair<yMin, edgeTableType>
public:
	// default construt an "empty" object
	Object();
	// construct an object given the set of vertices and polygons 
	Object(vector<Vector> v, vector<PolygonType> p);
	// destructor
	~Object();
	// return size of vectex in this object
	int sizeOfVertex();
	// return size of polygon in this object
	int sizeOfPolygon();
	// get vertex by index
	Vector getVertex(int i);
	// set vertex by index
	void setVertex(int i, Vector v);
	// get polygon by index
	PolygonType getPolygon(int i);
	// set polygon by index
	void setPolygon(int i, PolygonType p);
	// update yMin, yMax of polygon
	void updateYInfo();
	// randomly generate color for each polygons
	void randomColoring();
	// create edge table for each polygon
	void createEdgeTable();
	// perform scan convertion
	void scanConvertion();

};