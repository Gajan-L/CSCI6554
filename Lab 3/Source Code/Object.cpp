#include"pch.h"
#include"Object.h"
#include"Vertex.h"
#include"Vector.h"
#include"GlobalVariable.h"
#include"Illumination.h"

// default construt an "empty" object
Object::Object() {
	vertexNum = 0;
	polygonNum = 0;
	vertexOfObject.resize(0);
	polygonOfObject.resize(0);
	edgeTable.resize(0);
}

// construct an object with index of i and consisted of n polygons 
Object::Object(vector<Vertex> v, vector<PolygonType> p, int sType) {
	vertexNum = v.size() - 1;
	polygonNum = p.size();
	vertexOfObject = v;
	polygonOfObject = p;
	shadingType = sType;
	edgeTable.resize(polygonNum);
	for (int i = 0; i < polygonNum; i++) {
		for (int v : polygonOfObject[i].vertexOfPolygon) {
			vertexOfObject[v].updateAdjPolygon(i);
		}
	}
}

// destructor
Object::~Object() {
	vector<Vertex>().swap(vertexOfObject);
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
Vertex Object::getVertex(int i) {
	return vertexOfObject[i];
}

// set vertex by index
void Object::setVertex(int i, Vertex v) {
	vertexOfObject[i] = v;
}

// set vertex position given Vector transform position
void Object::setVertexTransformPosi(int i, Vector posi) {
	vertexOfObject[i].setTransformPosition(posi);
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
		for (int& v : p.vertexOfPolygon) {
			yMin = min(yMin, (int)vertexOfObject[v].getPositionAfterTransform().y);
			yMax = max(yMax, (int)vertexOfObject[v].getPositionAfterTransform().y);
		}
		p.yMin = yMin;
		p.yMax = yMax;
	}
}

// update normal of vertex
void Object::updateVertexNormal() {
	for (Vertex& v : vertexOfObject) {
		Vector normal;
		for (auto p : v.getAdjPolygon()) {
			normal = normal.add(polygonOfObject[p].normal);
		}
		normal.normalize();
		v.setNormal(normal);
	}
}

// randomly generate color for each polygons
Vector Object::randomColoring() {
	float colorR = rand() / float(RAND_MAX);
	float colorG = rand() / float(RAND_MAX);
	float colorB = rand() / float(RAND_MAX);
	return Vector(colorR, colorG, colorB);
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
			int yMin, yMax;
			float xMin, xoy;
			Vector headPosition = vertexOfObject[e.head].getPositionAfterTransform();
			Vector tailPosition = vertexOfObject[e.tail].getPositionAfterTransform();
			
			if (headPosition.y > tailPosition.y) {
				yMax = (int)headPosition.y;
				yMin = (int)tailPosition.y;
				xMin = tailPosition.x;
			}
			else {
				yMax = (int)tailPosition.y;
				yMin = (int)headPosition.y;
				xMin = headPosition.x;
			}
			
			xoy = (tailPosition.x - headPosition.x) / (tailPosition.y - headPosition.y);
			if (map.count(yMin)) {
				// if already have key yMin in map, append the information to the vector
				map[yMin].push_back(edgeTableType(e.head, e.tail, yMax, xMin, xoy));
			}
			else {
				// if not, add a new entry
				vector<edgeTableType> v;
				v.push_back(edgeTableType(e.head, e.tail, yMax, xMin, xoy));
				map[yMin] = v;
			}
		}
		for (auto& p : map) {
			curEdgeTable.push_back(p);
		}

		// sort the edge table based on yMin
		sort(curEdgeTable.begin(), curEdgeTable.end(), [](const pair<int, vector<edgeTableType>> e1, const pair<int, vector<edgeTableType>> e2) {return e1.first < e2.first; });
		edgeTable[i] = curEdgeTable;
	}
}

// perform scan convertion
void Object::scanConvertion() {
	updateVertexNormal();
	for (int i = 0; i < polygonNum; i++) {
		PolygonType curPolygon = polygonOfObject[i];
		if (curPolygon.visible) {
			// if current polygon is visible
			vector<pair<int, vector<edgeTableType>>> curEdgeTable = edgeTable[i];

			switch (shadingType) {
			case RANDOM:
				randomShading(curPolygon, curEdgeTable);
				break;
			case CONSTANT:
				constantShade(curPolygon, curEdgeTable);
				break;
			case GOURAUD:
				gourandShade(curPolygon, curEdgeTable);
				break;
			case PHONG:
				phongShade(curPolygon, curEdgeTable);
				break;
			}

		}
	}
}

// random shading
void Object::randomShading(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& curEdgeTable) {

	curPolygon.ambient = randomColoring();
	Vector color = curPolygon.ambient;
	// index of the curEdgeTable
	int index = 0;
	vector<edgeTableType> activeEdgeTable;
	for (int y = max(curPolygon.yMin, 0); y <= min(curPolygon.yMax, (int)screenHeight); y++) {
		if (index < curEdgeTable.size()) {
			if (y == curEdgeTable[index].first) {
				// if there is still edge in edge table and cur y is equal to yMin of the front element of the table
				for (edgeTableType e : curEdgeTable[index].second) {
					activeEdgeTable.push_back(e);
				}
				++index;
			}
		}

		// sort the active edge table based on xMin
		sort(activeEdgeTable.begin(), activeEdgeTable.end(), [](const edgeTableType& e1, const edgeTableType& e2) {return e1.xMin < e2.xMin; });

		// remove leave edges
		for (int k = 0; k < activeEdgeTable.size(); k++) {
			if (activeEdgeTable[k].yMax <= y) {
				activeEdgeTable.erase(activeEdgeTable.begin() + k);
				k--;
			}
		}

		assert(activeEdgeTable.size() % 2 == 0);
		for (int k = 0; k < activeEdgeTable.size() / 2; k += 2) {
			edgeTableType start = activeEdgeTable[k];
			edgeTableType end = activeEdgeTable[k + 1];

			// get posi after tranform
			Vector startHeadPosi = vertexOfObject[start.head].getPositionAfterTransform();
			Vector startTailPosi = vertexOfObject[start.tail].getPositionAfterTransform();
			Vector endHeadPosi = vertexOfObject[end.head].getPositionAfterTransform();
			Vector endTailPosi = vertexOfObject[end.tail].getPositionAfterTransform();

			float zStart = startHeadPosi.z - (startHeadPosi.z - startTailPosi.z) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float zEnd = endHeadPosi.z - (endHeadPosi.z - endTailPosi.z) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float xStart = startHeadPosi.x - (startHeadPosi.x - startTailPosi.x) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float xEnd = endHeadPosi.x - (endHeadPosi.x - endTailPosi.x) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float z = zStart;
			float deltaZ = (zStart - zEnd) / (xStart - xEnd);


			for (int x = max((int)xStart, 0); x <= min((int)xEnd, (int)screenWidth); x++) {
				if (z < zDepth[x][y]) {
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



	}
}

// constant shading
void Object::constantShade(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& curEdgeTable) {

	Vector color = illumination(ka, kd, ks, curPolygon.ambient, lightIntensity, curPolygon.center, lightSource, curPolygon.normal, cameraPosition, focusFactor); 
	// index of the curEdgeTable
	int index = 0;
	vector<edgeTableType> activeEdgeTable;
	for (int y = max(curPolygon.yMin, 0); y <= min(curPolygon.yMax, (int)screenHeight); y++) {
		if (index < curEdgeTable.size()) {
			if (y == curEdgeTable[index].first) {
				// if there is still edge in edge table and cur y is equal to yMin of the front element of the table
				for (edgeTableType &e : curEdgeTable[index].second) {
					activeEdgeTable.push_back(e);
				}
				++index;
			}
		}

		// sort the active edge table based on xMin
		sort(activeEdgeTable.begin(), activeEdgeTable.end(), [](const edgeTableType& e1, const edgeTableType& e2) {return e1.xMin < e2.xMin; });

		// remove leave edges
		for (int k = 0; k < activeEdgeTable.size(); k++) {
			if (activeEdgeTable[k].yMax <= y) {
				activeEdgeTable.erase(activeEdgeTable.begin() + k);
				k--;
			}
		}

		assert(activeEdgeTable.size() % 2 == 0);
		for (int k = 0; k < activeEdgeTable.size() / 2; k += 2) {
			edgeTableType start = activeEdgeTable[k];
			edgeTableType end = activeEdgeTable[k + 1];

			// get position after tranform
			Vector startHeadPosi = vertexOfObject[start.head].getPositionAfterTransform();
			Vector startTailPosi = vertexOfObject[start.tail].getPositionAfterTransform();
			Vector endHeadPosi = vertexOfObject[end.head].getPositionAfterTransform();
			Vector endTailPosi = vertexOfObject[end.tail].getPositionAfterTransform();

			// interpolate to get start and end position
			float zStart = startHeadPosi.z - (startHeadPosi.z - startTailPosi.z) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float zEnd = endHeadPosi.z - (endHeadPosi.z - endTailPosi.z) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float xStart = startHeadPosi.x - (startHeadPosi.x - startTailPosi.x) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float xEnd = endHeadPosi.x - (endHeadPosi.x - endTailPosi.x) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);

			// calculate delta of z
			float deltaZ = (zStart - zEnd) / (xStart - xEnd);

			// beginning state
			float z = zStart;

			for (int x = max((int)xStart, 0); x <= min((int)xEnd, (int)screenWidth); x++) {
				if (z < zDepth[x][y]) {
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

	}
}

// gourand shading
void Object::gourandShade(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& curEdgeTable) {
	// index of the curEdgeTable
	int index = 0;
	vector<edgeTableType> activeEdgeTable;
	for (int y = max(curPolygon.yMin, 0); y <= min(curPolygon.yMax, (int)screenHeight); y++) {
		if (index < curEdgeTable.size()) {
			if (y == curEdgeTable[index].first) {
				// if there is still edge in edge table and cur y is equal to yMin of the front element of the table
				for (edgeTableType &e : curEdgeTable[index].second) {
					activeEdgeTable.push_back(e);
				}
				++index;
			}
		}

		// sort the active edge table based on xMin
		sort(activeEdgeTable.begin(), activeEdgeTable.end(), [](const edgeTableType& e1, const edgeTableType& e2) {return e1.xMin < e2.xMin; });

		// remove leave edges
		for (int k = 0; k < activeEdgeTable.size(); k++) {
			if (activeEdgeTable[k].yMax <= y) {
				activeEdgeTable.erase(activeEdgeTable.begin() + k);
				k--;
			}
		}

		assert(activeEdgeTable.size() % 2 == 0);
		for (int k = 0; k < activeEdgeTable.size() / 2; k += 2) {
			edgeTableType start = activeEdgeTable[k];
			edgeTableType end = activeEdgeTable[k + 1];

			Vertex startHead = vertexOfObject[start.head];
			Vertex startTail = vertexOfObject[start.tail];
			Vertex endHead = vertexOfObject[end.head];
			Vertex endTail = vertexOfObject[end.tail];

			// get position after tranform
			Vector startHeadPosi = startHead.getPositionAfterTransform();
			Vector startTailPosi = startTail.getPositionAfterTransform();
			Vector endHeadPosi = vertexOfObject[end.head].getPositionAfterTransform();
			Vector endTailPosi = vertexOfObject[end.tail].getPositionAfterTransform();

			// get intensity of ends of both edge
			Vector iStartHead = illumination(ka, kd, ks, curPolygon.ambient, lightIntensity, startHead.getPosition(), lightSource, startHead.getNormal(), cameraPosition, focusFactor);
			Vector iStartTail = illumination(ka, kd, ks, curPolygon.ambient, lightIntensity, startTail.getPosition(), lightSource, startTail.getNormal(), cameraPosition, focusFactor);
			Vector iEndHead = illumination(ka, kd, ks, curPolygon.ambient, lightIntensity, endHead.getPosition(), lightSource, endHead.getNormal(), cameraPosition, focusFactor);
			Vector iEndTail = illumination(ka, kd, ks, curPolygon.ambient, lightIntensity, endTail.getPosition(), lightSource, endTail.getNormal(), cameraPosition, focusFactor);

			// interpolate to get the start and end intensity
			float iStartR = iStartHead.x * (y - startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + iStartTail.x * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float iStartG = iStartHead.y * (y - startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + iStartTail.y * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float iStartB = iStartHead.z * (y - startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + iStartTail.z * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float iEndR = iEndHead.x * (y - endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + iEndTail.x * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float iEndG = iEndHead.y * (y - endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + iEndTail.y * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float iEndB = iEndHead.z * (y - endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + iEndTail.z * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);

			// interpolate to get the start and end position
			float zStart = startHeadPosi.z - (startHeadPosi.z - startTailPosi.z) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float zEnd = endHeadPosi.z - (endHeadPosi.z - endTailPosi.z) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float xStart = startHeadPosi.x - (startHeadPosi.x - startTailPosi.x) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float xEnd = endHeadPosi.x - (endHeadPosi.x - endTailPosi.x) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);

			// calculate delta of z and delta of color
			float deltaZ = (zStart - zEnd) / (xStart - xEnd);
			float deltaCR = (iStartR - iEndR) / (xStart - xEnd);
			float deltaCG = (iStartG - iEndG) / (xStart - xEnd);
			float deltaCB = (iStartB - iEndB) / (xStart - xEnd);
			Vector deltaC = Vector(deltaCR, deltaCG, deltaCB);

			// beginning state
			float z = zStart;
			Vector color = Vector(iStartR, iStartG, iStartB);

			for (int x = max((int)xStart, 0); x <= min((int)xEnd, (int)screenWidth); x++) {
				if (z < zDepth[x][y]) {
					zDepth[x][y] = z;
					zBuffer[x][y] = color;
				}
				z += deltaZ;
				color = color.add(deltaC);
			}
		}


		// update xMin
		for (int k = 0; k < activeEdgeTable.size(); k++) {
			activeEdgeTable[k].xMin += activeEdgeTable[k].xoy;
		}

	}
}

// phong shading
void Object::phongShade(PolygonType& curPolygon, vector<pair<int, vector<edgeTableType>>>& curEdgeTable){
	// index of the curEdgeTable
	int index = 0;
	vector<edgeTableType> activeEdgeTable;
	for (int y = max(curPolygon.yMin, 0); y <= min(curPolygon.yMax, (int)screenHeight); y++) {
		if (index < curEdgeTable.size()) {
			if (y == curEdgeTable[index].first) {
				// if there is still edge in edge table and cur y is equal to yMin of the front element of the table
				for (edgeTableType e : curEdgeTable[index].second) {
					activeEdgeTable.push_back(e);
				}
				++index;
			}
		}

		// sort the active edge table based on xMin
		sort(activeEdgeTable.begin(), activeEdgeTable.end(), [](const edgeTableType& e1, const edgeTableType& e2) {return e1.xMin < e2.xMin; });

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

			Vertex startHead = vertexOfObject[start.head];
			Vertex startTail = vertexOfObject[start.tail];
			Vertex endHead = vertexOfObject[end.head];
			Vertex endTail = vertexOfObject[end.tail];

			// get position after tranform
			Vector startHeadPosi = startHead.getPositionAfterTransform();
			Vector startTailPosi = startTail.getPositionAfterTransform();
			Vector endHeadPosi = endHead.getPositionAfterTransform();
			Vector endTailPosi = endTail.getPositionAfterTransform();

			// get position in world coordinate
			Vector startHeadPosiInWorld = startHead.getPosition();
			Vector startTailPosiInWorld = startTail.getPosition();
			Vector endHeadPosiInWorld = endHead.getPosition();
			Vector endTailPosiInWorld = endTail.getPosition();

			// get normal of ends of both edges
			Vector startHeadNormal = startHead.getNormal();
			Vector startTailNormal = startTail.getNormal();
			Vector endHeadNormal = endHead.getNormal();
			Vector endTailNormal = endTail.getNormal();
			
			// interpolate to get the start and end position
			float zStart = startHeadPosi.z - (startHeadPosi.z - startTailPosi.z) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float zEnd = endHeadPosi.z - (endHeadPosi.z - endTailPosi.z) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			int xStart = startHeadPosi.x - (startHeadPosi.x - startTailPosi.x) * (startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			int xEnd = endHeadPosi.x - (endHeadPosi.x - endTailPosi.x) * (endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);

			// interpolate to get the start and end position in world coordinate
			float startX = startHeadPosiInWorld.x * (y - (int)startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + startTailPosiInWorld.x * ((int)startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float startY = startHeadPosiInWorld.y * (y - (int)startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + startTailPosiInWorld.y * ((int)startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float startZ = startHeadPosiInWorld.z * (y - (int)startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + startTailPosiInWorld.z * ((int)startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float endX = endHeadPosiInWorld.x * (y - (int)endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + endTailPosiInWorld.x * ((int)endHeadPosi.y - y) / ((int)endHeadPosi.y - endTailPosi.y);
			float endY = endHeadPosiInWorld.y * (y - (int)endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + endTailPosiInWorld.y * ((int)endHeadPosi.y - y) / ((int)endHeadPosi.y - endTailPosi.y);
			float endZ = endHeadPosiInWorld.z * (y - (int)endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + endTailPosiInWorld.z * ((int)endHeadPosi.y - y) / ((int)endHeadPosi.y - endTailPosi.y);

			// interpolate to get the start normal and end normal
			float startNormalx = startHeadNormal.x * (y - (int)startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + startTailNormal.x * ((int)startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float startNormaly = startHeadNormal.y * (y - (int)startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + startTailNormal.y * ((int)startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float startNormalz = startHeadNormal.z * (y - (int)startTailPosi.y) / (startHeadPosi.y - startTailPosi.y) + startTailNormal.z * ((int)startHeadPosi.y - y) / (startHeadPosi.y - startTailPosi.y);
			float endNormalx = endHeadNormal.x * (y - (int)endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + endTailNormal.x * ((int)endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float endNormaly = endHeadNormal.y * (y - (int)endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + endTailNormal.y * ((int)endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);
			float endNormalz = endHeadNormal.z * (y - (int)endTailPosi.y) / (endHeadPosi.y - endTailPosi.y) + endTailNormal.z * ((int)endHeadPosi.y - y) / (endHeadPosi.y - endTailPosi.y);

			// calculate delta of z, normal and point
			float deltaZ = (zStart - zEnd) / (xStart - xEnd);
			float deltaNormalx = (startNormalx - endNormalx) / (xStart - xEnd);
			float deltaNormaly = (startNormaly - endNormaly) / (xStart - xEnd);
			float deltaNormalz = (startNormalz - endNormalz) / (xStart - xEnd);
			float deltaXInWorld = (startX - endX) / (xStart - xEnd);
			float deltaYInWorld = (startY - endY) / (xStart - xEnd);
			float deltaZInWorld = (startZ - endZ) / (xStart - xEnd);
			Vector deltaN = Vector(deltaNormalx, deltaNormaly, deltaNormalz);
			Vector deltaP = Vector(deltaXInWorld, deltaYInWorld, deltaZInWorld);

			// beginning state
			float z = zStart;
			float curNormalx = startNormalx;
			float curNormaly = startNormaly;
			float curNormalz = startNormalz;
			Vector curNormal = Vector(startNormalx, startNormaly, startNormalz);
			Vector curPoint = Vector(startX, startY, startZ);


			for (int x = max((int)xStart, 0); x <= min((int)xEnd, (int)screenWidth); x++) {
				if (z <= zDepth[x][y]) {
					Vector color = illumination(ka, kd, ks, curPolygon.ambient, lightIntensity, curPoint, lightSource, curNormal, cameraPosition, focusFactor);
					zDepth[x][y] = z;
					zBuffer[x][y] = color;
				}
				z += deltaZ;
				curNormal = curNormal.add(deltaN);
				curPoint = curPoint.add(deltaP);
			}
		}


		// update xMin
		for (int k = 0; k < activeEdgeTable.size(); k++) {
			activeEdgeTable[k].xMin += activeEdgeTable[k].xoy;
		}

	}
}