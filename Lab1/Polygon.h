#pragma once
#include<vector>
using namespace std;

struct line {
	int head;	// head node
	int tail;	// tail node
};

struct polygon {
	int v_num;			// num of vertices
	vector<line> lines;	// lines in the polygon
	bool visible;		// whether this polygon is visible or not
};

class Polygons {
private:
	int p_num;				// num of polygons;
	vector<polygon> poly;	// polygons
public:
	// default construt an empty set of polygon
	Polygons() {
		p_num = 0;
		poly.resize(0);
	}
	// construct a set a polygons with size of n
	Polygons(int n) {
		p_num = n;
		poly.resize(n);
	}
	// get the size of the set of polygons
	int size() {
		return p_num;
	}
	// get polygon by index
	polygon get(int i) {
		return poly[i];
	}
	// set the polygon of index i
	void set(int i, polygon p) {
		poly[i] = p;
	}
};