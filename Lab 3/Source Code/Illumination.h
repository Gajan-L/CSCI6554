#pragma once
class Vector;

// get ambient term
Vector getAmbient(float ka, Vector ambientIntensity);

// get diffuse term
Vector getDiffuse(float kd, Vector normal, Vector point, Vector lightSource, Vector lightIntensity);

// get specular term
Vector getSpecular(float ks, Vector normal, Vector point, Vector lightSource, Vector lightIntensity, Vector camera, int focusingFactor);

// get final color after illumination
Vector illumination(float ka, float kd, float ks, Vector ambientIntensity, Vector lightIntensity, Vector point, Vector lightSource, Vector normal, Vector viewer, int focusingFactor);