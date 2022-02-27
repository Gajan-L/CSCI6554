#include"pch.h"

// default construt an "empty" object
Object::Object() {
	vertexNum = 0;
	polygonNum = 0;
	vertexOfObject.resize(0);
	polygonOfObject.resize(0);
	edgeTable.resize(0);
}

// construct an object with index of i and consisted of n polygons 
Object::Object(vector<Vector> v, vector<PolygonType> p) {
	vertexNum = v.size() - 1;
	polygonNum = p.size();
	vertexOfObject = v;
	polygonOfObject = p;
	edgeTable.resize(polygonNum);
}

// destructor
Object::~Object() {
	vector<Vector>().swap(vertexOfObject);
	vector<PolygonType>().swap(polygonOfObject);
	vector<vector<pair<int, vector<edgeTableType>>>>().swap(edgeTable);
}

// return size of vectex in this object
int Object::sizeOfVertex() {
	return vertexNum;
}

// return size of polygon in this object
int Object::sizeOfPolygon() {
	return polygonNum;
}

// get vertex by index
Vector Object::getVertex(int i) {
	return vertexOfObject[i];
}
// set vertex by index
void Object::setVertex(int i, Vector v) {
	vertexOfObject[i] = v;
}

// get polygon by index i 
PolygonType Object::getPolygon(int i) {
	return polygonOfObject[i];
}

// set polygon by index
void Object::setPolygon(int i, PolygonType p) {
	polygonOfObject[i] = p;
}

// update yMin, yMax of polygon
void Object::updateYInfo() {
	for (PolygonType& p : polygonOfObject) {
		int yMin = INT_MAX;
		int yMax = INT_MIN;
		for (int v : p.vertexOfPolygon) {
			yMin = min(yMin, (int)vertexOfObject[v].y);
			yMax = max(yMax, (int)vertexOfObject[v].y);
		}
		p.yMin = yMin;
		p.yMax = yMax;
	}
}

// randomly generate color for each polygons
void Object::randomColoring() {
	srand((unsigned int)time(0));
	for (PolygonType& p : polygonOfObject) {
		p.colorR = rand() / float(RAND_MAX);
		p.colorG = rand() / float(RAND_MAX);
		p.colorB = rand() / float(RAND_MAX);
	}
}

// create edge table for each polygon
void Object::createEdgeTable() {
	for (int i = 0; i < polygonNum; i++) {
		PolygonType curPolygon = polygonOfObject[i];
		vector<pair<int, vector<edgeTableType>>> curEdgeTable;

		// use a hashtable to map edges that share the same yMin
		unordered_map<int, vector<edgeTableType>> map;
		// for every edge, calculate the yMin, yMax, xMin and xoy;
		for (int j = 0; j < curPolygon.edgeOfPolygon.size(); j++) {
			Edge e = curPolygon.edgeOfPolygon[j];
			Vector head = vertexOfObject[e.head];
			Vector tail = vertexOfObject[e.tail];
			int yMin, yMax;
			float xMin, xoy;
			if (head.y > tail.y) {
				yMax = (int)head.y;
				yMin = (int)tail.y;
				xMin = tail.x;
				xoy = (tail.x - head.x) / (tail.y - head.y);
			}
			else {
				yMax = (int)tail.y;
				yMin = (int)head.y;
				xMin = head.x;
				xoy = (tail.x - head.x) / (tail.y - head.y);
			}
			if (map.count(yMin)) {
				// if already have key yMin in map, append the information to the vector
				map[yMin].push_back(edgeTableType(head, tail, yMax, xMin, xoy));
			}
			else {
				// if not, add a new entry
				vector<edgeTableType> v;
				v.push_back(edgeTableType(head, tail, yMax, xMin, xoy));
				map[yMin] = v;
			}
		}
		for (auto p : map) {
			curEdgeTable.push_back(p);
		}

		// sort the edge table based on yMin
		sort(curEdgeTable.begin(), curEdgeTable.end(), [](const pair<int, vector<edgeTableType>> e1, const pair<int, vector<edgeTableType>> e2) {return e1.first < e2.first; });
		edgeTable[i] = curEdgeTable;
	}
} 

// perform scan convertion
void Object::scanConvertion() {	
	for (int i = 0; i < polygonNum; i++) {
		PolygonType curPolygon = polygonOfObject[i];
		if (curPolygon.visible) {
			// if current polygon is visible
			Vector color = Vector(curPolygon.colorR, curPolygon.colorG, curPolygon.colorB);
			vector<pair<int, vector<edgeTableType>>> curEdgeTable = edgeTable[i];

			// index of the curEdgeTable
			int index = 0;
			vector<edgeTableType> activeEdgeTable;
			for (int y = max(curPolygon.yMin, 0); y <= min(curPolygon.yMax + 1, (int)screenHeight); y++) {
				if (index < curEdgeTable.size()) {
					if (y == curEdgeTable[index].first) {
						// if there is still edge in edge table and cur y is equal to yMin of the front element of the table
						for (edgeTableType e : curEdgeTable[index].second) {
							activeEdgeTable.push_back(e);
						}
						++index;
					}
				}

				// remove leave edges
				for (int k = 0; k < activeEdgeTable.size(); k++) {
					if (activeEdgeTable[k].yMax == y) {
						activeEdgeTable.erase(activeEdgeTable.begin() + k);
						k--;
					}
				}

				assert(activeEdgeTable.size() % 2 == 0);
				for (int k = 0; k < activeEdgeTable.size() / 2; k += 2) {
					edgeTableType start = activeEdgeTable[k];
					edgeTableType end = activeEdgeTable[k + 1];
					float zStart = start.head.z - (start.head.z - start.tail.z) * (start.head.y - y) / (start.head.y - start.tail.y);
					float zEnd = end.head.z - (end.head.z - end.tail.z) * (end.head.y - y) / (end.head.y - end.tail.y);
					float z = zStart;
					float deltaZ = (zStart - zEnd) / (start.xMin - end.xMin);
					for (int x = max((int)start.xMin, 0); x <= min((int)end.xMin, (int)screenWidth); x++) {
						if (z <= zDepth[x][y]) {
							zDepth[x][y] = z;
							zBuffer[x][y] = color;
						}
						z += deltaZ;
					}
				}

				// update xMin
				for (int k = 0; k < activeEdgeTable.size(); k++) {
					activeEdgeTable[k].xMin += activeEdgeTable[k].xoy;
				}

				// sort the active edge table based on xMin
				sort(activeEdgeTable.begin(), activeEdgeTable.end(), [](const edgeTableType& e1, const edgeTableType& e2) {return e1.xMin < e2.xMin; });

			}
		}
	}
}