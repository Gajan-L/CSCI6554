#pragma once
#include<vector>
#include"Vector.h"
using namespace std;

class Vertex {
private:
	int v_num;
	vector<Vector> vertices;
public:
	// default construct a empty set of vertex
	Vertex() {	
		v_num = 0;
		vertices.resize(0);
	}
	// construct a set of vertices with size of n
	Vertex(int n) {
		v_num = n;
		vertices.resize(n);
	}
	// get the size of set of vertrices
	int size() {	
		return v_num;
	}
	// get vertex with index i
	Vector get(int i) {	
		return vertices[i];
	}
	// modify vertex of index i
	void set(int i, Vector v) {	
		vertices[i] = v;
	}
};