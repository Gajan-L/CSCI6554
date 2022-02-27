#pragma once
#include"Matrix.h"
#include"Vector.h"

class Camera {
private:
	Matrix view;
	Vector position, view_direction, Up, N, V, U;
public:
	Camera(){
		view = Matrix();
		position = Vector();
		view_direction = Vector();
		Up = Vector();
		N = Vector();
		V = Vector();
		U = Vector();
	}
	// set the coordinate from global varibles
	void setCoordinate(Vector posi, Vector vd, Vector up) {	
		position = posi;
		view_direction = vd;
		Up = up;
		N = Vector(vd.getX() - posi.getX(), vd.getY() - posi.getY(), vd.getZ() - posi.getZ());
		N.normalize();
		U = N.crossProduct(Up);
		U.normalize();
		V = U.crossProduct(N);
	}
	// set the view matrix
	void setView() {	
		// rotation matrix
		Matrix R = Matrix(4);	
		// translate matrix
		Matrix T = Matrix(4);	
		R.modify(0, 0, U.getX());
		R.modify(0, 1, U.getY());
		R.modify(0, 2, U.getZ());
		R.modify(1, 0, V.getX());
		R.modify(1, 1, V.getY());
		R.modify(1, 2, V.getZ());
		R.modify(2, 0, N.getX());
		R.modify(2, 1, N.getY());
		R.modify(2, 2, N.getZ());
		T.modify(0, 3, -position.getX());
		T.modify(1, 3, -position.getY());
		T.modify(2, 3, -position.getZ());

		view = R.mul(T);
	}
	Matrix getView() {
		return view;
	}
};