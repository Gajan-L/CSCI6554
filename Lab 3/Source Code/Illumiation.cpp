#include"pch.h"
#include"Vector.h"
#include"Illumination.h"

// get ambient term
Vector getAmbient(float ka, Vector ambientIntensity) {
	ambientIntensity.scalarMul(ka);
	return ambientIntensity;
}

// get diffuse term
Vector getDiffuse(float kd, Vector normal, Vector point, Vector lightSource, Vector lightIntensity) {
	//Vector L = point.sub(lightSource);
	Vector L = lightSource.sub(point);
	L.normalize();
	lightIntensity.scalarMul(normal.dotProduct(L));
	lightIntensity.scalarMul(kd);
	return lightIntensity;
}

// get specular term
Vector getSpecular(float ks, Vector normal, Vector point, Vector lightSource, Vector lightIntensity, Vector viewer, int focusingFactor) {
	Vector L = lightSource.sub(point);
	L.normalize();
	Vector H = L.add(viewer.sub(point));
	H.normalize();

	float factor = pow(normal.dotProduct(H), focusingFactor); 
	lightIntensity.scalarMul(factor);
	lightIntensity.scalarMul(ks);
	return lightIntensity;
}

// get final color after illumination
Vector illumination(float ka, float kd, float ks, Vector ambientIntensity, Vector lightIntensity, Vector point, Vector lightSource, Vector normal, Vector viewer, int focusingFactor) {
	viewer.normalize();
	normal.normalize();

	Vector ambient = getAmbient(ka, ambientIntensity);
	Vector diffuse = getDiffuse(kd, normal, point, lightSource, lightIntensity);
	Vector specular = getSpecular(ks, normal, point, lightSource, lightIntensity, viewer, focusingFactor);

	return ambient.add(diffuse.add(specular));
	//return specular;
}