#include"pch.h"
#include"Vertex.h"
#include"Vector.h"
#include"GlobalVariable.h"
#include"Object.h"
#include"Matrix.h"
#include"Function.h"
#include"Camera.h"

// preprocessing input date by removing all '\t'
string preprocess(string& s, const string& old, const string& replacement) {
	string::size_type posi(0);
	while ((posi = s.find(old)) != string::npos) {
		s.replace(posi, old.length(), replacement);
	}
	return s;
}

// split string by the split char
vector<string> split(const string& s, char p) {
	auto start = 0;
	auto length = s.length();
	string t;
	vector<string> tmp;
	if (tmp.size() > 0)
		tmp.clear();
	for (auto i = 0; i < length; i++) {
		if (s[i] == p && i == 0)
			start++;
		else if (s[i] == p) {
			t = s.substr(start, i - start);
			if (!t.empty()) {
				tmp.push_back(t);
			}
			start = i + 1;
		}
		else if (i == length - 1) {
			t = s.substr(start, i - start + 1);
			if (!t.empty()) {
				tmp.push_back(t);
			}
			start = i + 1;
		}
	}
	return tmp;
}

// load data from file
void loadData(string file, shadingType st, Vector ambient) {
	ifstream ifs;
	ifs.open(file, ios::in);

	vector<string> s;
	string buf;
	while (!ifs.eof()) {
		getline(ifs, buf);
		buf = preprocess(buf, "\t", " ");
		s.push_back(buf);
	}

	int vertexNum = stoi(split(s[0], ' ')[1]);
	int polygonNum = stoi(split(s[0], ' ')[2]);

	// vectex is 1-indexed
	vector<Vertex> vertex(vertexNum + 1);
	// polygon is 0-indexed
	vector<PolygonType> polygon(polygonNum);

	// create set of vertex
	for (int i = 1; i <= vertexNum; i++) {
		float x, y, z;
		stringstream s0(split(s[i], ' ')[0]);
		stringstream s1(split(s[i], ' ')[1]);
		stringstream s2(split(s[i], ' ')[2]);
		s0 >> x;
		s1 >> y;
		s2 >> z;
		vertex[i] = Vertex(x, y, z);
	}

	for (int i = 0; i < polygonNum; i++) {
		PolygonType p;
		int vertexNumOfPolygon = stoi(split(s[i + vertexNum + 1], ' ')[0]);
		float xMin = FLT_MAX, xMax = FLT_MIN, yMin = FLT_MAX, yMax = FLT_MIN, zMin = FLT_MAX, zMax = FLT_MIN;
		p.edgeOfPolygon.resize(vertexNumOfPolygon);
		p.vertexOfPolygon.resize(vertexNumOfPolygon);

		// load data of vextex of polygon and yMin, yMax;
		for (int k = 0; k < vertexNumOfPolygon; k++) {
			p.vertexOfPolygon[k] = stoi(split(s[i + vertexNum + 1], ' ')[k + 1]);
			vertex[p.vertexOfPolygon[k]].updateAdjPolygon(i);
			Vector posi = vertex[p.vertexOfPolygon[k]].getPosition();
			xMin = min(xMin, posi.x);
			xMax = max(xMax, posi.x);
			yMin = min(yMin, posi.y);
			yMax = max(yMax, posi.y);
			zMin = min(zMin, posi.z);
			zMax = max(zMax, posi.z);
		}
		for (int k = 0; k < vertexNumOfPolygon; k++) {
			int head = p.vertexOfPolygon[k];
			int tail = p.vertexOfPolygon[(k + 1) % vertexNumOfPolygon];
			p.edgeOfPolygon[k] = Edge(head, tail);
		}

		// add the last edge that starts from the last node and ends with the first node
		//p.edgeOfPolygon[vertexNumOfPolygon - 1] = Edge(p.vertexOfPolygon[vertexNumOfPolygon - 1], p.vertexOfPolygon[0]);

		// calculate the normal of this polygon
		Edge e1 = p.edgeOfPolygon[0];
		Edge e2 = p.edgeOfPolygon[1];
		Vector edge1 = vertex[e1.head].getPosition().sub(vertex[e1.tail].getPosition());
		Vector edge2 = vertex[e2.head].getPosition().sub(vertex[e2.tail].getPosition());
		p.normal = edge2.crossProduct(edge1);
		//p.normal = edge1.crossProduct(edge2);
		p.normal.normalize();
		p.ambient = ambient;
		p.center = Vector((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);

		polygon[i] = p;
	}
	objects.push_back(Object(vertex, polygon, st));
}


// perspective matrix
Matrix perspective() {
	Matrix p = Matrix();
	p.modify(0.0, 0.0, d / h);
	p.modify(1.0, 1.0, d / h);
	p.modify(2.0, 2.0, f / (f - d));
	p.modify(2.0, 3.0, -d * f / (f - d));
	p.modify(3.0, 2.0, 1.0);
	return p;
}

// local to screen transformation
Vector transformation(Vector v) {
	Matrix pers = perspective();
	Matrix view = camera.getView();
	Matrix vertex = Matrix(v);
	Matrix res = pers.mul(view.mul(vertex));
	return res.fitToScreen(screenWidth, screenHeight);
}

// perform the transformation to vertices of each object
void performTransformation() {
	for (int i = 0; i < objects.size(); i++) {
		for (int j = 1; j <= objects[i].sizeOfVertex(); j++) {
			objects[i].setVertexTransformPosi(j, transformation(objects[i].getVertex(j).getPosition()));
		}
	}
}

// perform back face culling after transformation
void backFaceCulling() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i];
		for (int j = 0; j < objects[i].sizeOfPolygon(); j++) {
			PolygonType p = objects[i].getPolygon(j);

			// calculate normal of this polygon
			Edge e1 = p.edgeOfPolygon[0];
			Edge e2 = p.edgeOfPolygon[1];
			Vector edge1 = objects[i].getVertex(e1.head).getPositionAfterTransform().sub(objects[i].getVertex(e1.tail).getPositionAfterTransform());
			Vector edge2 = objects[i].getVertex(e2.head).getPositionAfterTransform().sub(objects[i].getVertex(e2.tail).getPositionAfterTransform());
			Vector normal = edge1.crossProduct(edge2);
			if (normal.z > 0) {
				p.visible = false;
			}
			else {
				p.visible = true;
			}
			objects[i].setPolygon(j, p);
		}
	}
}

// perform scan convertion
void performScanConvertion() {
	for (Object &o : objects) {
		o.updateYInfo();
		o.createEdgeTable();
		o.scanConvertion();
	}
}
