#include"GL/glut.h"
#include<GL/GL.h>
#include<iostream>
#include"GlobalVariable.h"
#include"Function.h"
#include"Matrix.h"
#include"Vector.h"

using namespace std;

void display() {
	Camera a;
	a.setCoordinate(camera_position, view_direction, UP);
	a.setView();
	float x, y;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f); 
	glBegin(GL_LINES);

	for (int i = 0; i < polygons.size(); i++) {
		// if not using back face culling, the visibility of polygons will not need to be considered
		// otherwise, only visible polygons will be rendered
		if (!with_culling || polygons.get(i).visible) {
			for (int j = 0; j < polygons.get(i).v_num; j++) {
				x = vertices.get(polygons.get(i).lines[j].head - 1).getX();
				y = vertices.get(polygons.get(i).lines[j].head - 1).getY();
				glVertex2f(x, y);
				x = vertices.get(polygons.get(i).lines[j].tail - 1).getX();
				y = vertices.get(polygons.get(i).lines[j].tail - 1).getY();
				glVertex2f(x, y);
			}
		}
	}
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {

	loadDate();
	perform();
	back_face_culling();

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Lab1");
	glutDisplayFunc(&display);
	glutMainLoop();

	return 0;
}