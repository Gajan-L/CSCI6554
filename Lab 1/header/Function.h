#pragma once
#include<fstream>
#include<sstream>
#include"Matrix.h"
#include"Vector.h"
#include"Camera.h"
#include"GlobalVariable.h"

// get the perspective matrix
Matrix perspective() { 
	Matrix p = Matrix();
	p.modify(0, 0, d / h);
	p.modify(1, 1, d / h);
	p.modify(2, 2, f / (f - d));
	p.modify(2, 3, -d * f / (f - d));
	p.modify(3, 2, 1);
	return p;
}

// local to screen transformation
Vector transformation(Camera c, Vector v) {	 
	Matrix res = perspective();
	res = res.mul(c.getView());
	res = res.mul(Matrix(v));
	return res.toVector();
}

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
		else if (i == length - 1){
			t = s.substr(start, i - start + 1);
			if (!t.empty()){
				tmp.push_back(t);
			}
			start = i + 1;
		}
	}
	return tmp;
}

// load date from file
void loadDate() {	
	ifstream ifs;
	ifs.open(file, ios::in);

	vector<string> s;
	string buf;
	while (!ifs.eof()) {
		getline(ifs, buf);
		buf = preprocess(buf, "\t", " ");
		s.push_back(buf);
	}

	v_num = stoi(split(s[0], ' ')[1]);
	p_num = stoi(split(s[0], ' ')[2]);
	vertices = Vertex(v_num);
	polygons = Polygons(p_num);

	for (int i = 0; i < vertices.size(); i++) {
		float x, y, z;
		stringstream s0(split(s[i + 1], ' ')[0]);
		stringstream s1(split(s[i + 1], ' ')[1]);
		stringstream s2(split(s[i + 1], ' ')[2]);
		s0 >> x;
		s1 >> y;
		s2 >> z;
		Vector tmp = Vector(x, y, z);
		vertices.set(i, tmp);
	}

	for (int i = 0; i < p_num; i++) {
		polygon p;
		p.v_num = stoi(split(s[i + v_num + 1], ' ')[0]);
		p.lines.resize(p.v_num);
		for (int j = 0; j < p.v_num - 1; j++) {
			p.lines[j].head = stoi(split(s[i + v_num + 1], ' ')[j + 1]);
			p.lines[j].tail = stoi(split(s[i + v_num + 1], ' ')[j + 2]);
		}
		p.lines[p.v_num - 1].head = stoi(split(s[i + v_num + 1], ' ')[p.v_num]);
		p.lines[p.v_num - 1].tail = stoi(split(s[i + v_num + 1], ' ')[1]);
		polygons.set(i, p);
	}
}

// perform the transformation to each vertices
void perform() {	
	camera.setCoordinate(camera_position, view_direction, UP);
	camera.setView();
	for (int i = 0; i < v_num; i++) {
		vertices.set(i, transformation(camera, vertices.get(i)));
	}
}

// perform back face culling after transformation
void back_face_culling() {	
	for (int i = 0; i < p_num; i++) {
		Vector v1, v2, v3, line1, line2, normal;
		v1 = vertices.get(polygons.get(i).lines[0].head - 1);
		v2 = vertices.get(polygons.get(i).lines[0].tail - 1);
		v3 = vertices.get(polygons.get(i).lines[1].tail - 1);
		line1 = v2.sub(v1);
		line2 = v3.sub(v2);
		normal = line1.crossProduct(line2);
		if (normal.getZ() > 0) {
			polygon  p = polygons.get(i);
			p.visible = false;
			polygons.set(i, p);
		}
	}
}