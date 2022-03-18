#pragma once
class Vector;

class Matrix {
public:
    // create an all-zero matrix of size 4 as default construction
    Matrix();
    // create an identity matrix of size n
    Matrix(int n);
    // create a matrix based on Vector
    Matrix(Vector v);
    // create a all-zero matrix of m*n
    Matrix(int m, int n);
    // create a rectangular matrix with m rows and n columns
    // array A[] stores the elements of the matrix
    Matrix(float A[], int m, int n);
    ~Matrix();
    // modify the matrix value in position (m, n) with the new value a
    void modify(int m, int n, float a);
    // scalar addition
    Matrix scalarAdd(float a);
    // scalar multiplication
    Matrix scalarMul(float factor);
    // matrix addition
    Matrix add(Matrix b);
    // matrix multiplication
    Matrix mul(Matrix b);
    // get the sizes of the matrix
    int nRows();
    int nColumns();
    // get the value of elements in the position (m, n)
    float get(int m, int n);
    // convert 1D-matrix to Vector
    // modify x,y value so that they coordinate with screen
    // x:[0,width]
    // y:[0,height]
    Vector fitToScreen(int width, int height);
private:
    // using vector array for representing the matrix
    vector<vector<float>> matrix;
};