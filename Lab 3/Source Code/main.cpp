#include"pch.h"
#include"Object.h"
#include"Function.h"
#include"GlobalVariable.h"
#include"Vertex.h"
#include"Vector.h"

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);

	float x, y;

	for (Object o : objects) {
		for (int i = 0; i < o.sizeOfPolygon(); i++) {
			// if not using back face culling, the visibility of polygons will not need to be considered
			// otherwise, only visible polygons will be rendered
			PolygonType p = o.getPolygon(i);
			if (!with_culling || p.visible) {
				for (int j = 0; j < p.edgeOfPolygon.size(); j++) {
					x = o.getVertex(p.edgeOfPolygon[j].head).getPositionAfterTransform().x;
					y = o.getVertex(p.edgeOfPolygon[j].head).getPositionAfterTransform().y;
					glVertex2f(x, y);
					x = o.getVertex(p.edgeOfPolygon[j].tail).getPositionAfterTransform().x;
					y = o.getVertex(p.edgeOfPolygon[j].tail).getPositionAfterTransform().y;
					glVertex2f(x, y);
				}
			}
		}
	}
	glEnd();
	glFlush();
}

void displayWithZBuffer() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (int i = 0; i <= screenWidth; i++) {
		for (int j = 0; j <= screenHeight; j++) {
			glVertex2i(i, j);
			glColor3f(zBuffer[i][j].x, zBuffer[i][j].y, zBuffer[i][j].z);
		}
	}

	glEnd();
	glFinish();
	glFlush();

}

void Init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)screenWidth, 0.0, (double)screenHeight);
	srand((unsigned int)time(0));
}

int main(int argc, char** argv) {
	cout << "Loading data...\n";
	loadData("D files/better-ball.d.txt", PHONG, Vector(0, 0.1, 1));
	cout << "Data loaded!\nRendering in process...\n";
	performTransformation();
	backFaceCulling();
	performScanConvertion();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Phong");
	Init();

	//glutDisplayFunc(&display);
	glutDisplayFunc(&displayWithZBuffer);
	cout << "Display!\n";
	glutMainLoop();

	return 0;
}