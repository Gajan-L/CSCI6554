#pragma once
#include<iostream>
#include <vector>
#include"Vector.h"

using namespace std;

class Matrix {
public:
    // create an all-zero matrix of size 4 as default construction
    Matrix() {
        matrix.resize(4);
        for (int i = 0; i < 4; i++) {
            // create a row of 4 element with the value 0
            vector<float> temp(4, 0);
            // add the row to the matrix
            matrix[i] = temp;
        }
    }
    // create an identity matrix of size n
    Matrix(int n) {
        matrix.resize(n);
        for (int i = 0; i < n; i++) {
            // create a row of n element with the value 0
            vector<float> temp(n, 0);
            // set the diagonal to 1
            temp[i] = 1;
            // add the row to the matrix
            matrix[i] = temp;
        }
    }
    // create a matrix based on vector
    Matrix(Vector v) {
        matrix.resize(4);
        for (int i = 0; i < 4; i++) {
            matrix[i].resize(1);
        }
        matrix[0][0] = v.getX();
        matrix[1][0] = v.getY();
        matrix[2][0] = v.getZ();
        matrix[3][0] = 1;
    }
    // create a all-zero matrix of m*n
    Matrix(int m, int n) {
        matrix.resize(m);
        for (int i = 0; i < m; i++) {
            // create a row of n element with the value 0
            vector<float> temp(n, 0);
            // add the row to the matrix
            matrix[i] = temp;
        }
    }
    // create a rectangular matrix with m rows and n columns
    // array A[] stores the elements of the matrix
    Matrix(float A[], int m, int n) {
        matrix.resize(m);
        for (int i = 0; i < m; i++) {
            // create a row of n element and assign the values from array A[]
            vector<float> temp(A + i * n, A + (i + 1) * n);
            // add the row to the matrix
            matrix[i] = temp;
        }
    }
    // modify the matrix value in position (m, n) with the new value a
    void modify(int m, int n, float a) {
        if (m >= matrix.size()) {
            cout << "Row number does not match\n";
        }
        else if (n >= matrix[m].size()) {
            cout << "Column number does not match\n";
        }
        else {
            matrix[m][n] = a;
        }
    }
    // scalar addition
    Matrix scalarAdd(float a) {
        Matrix r;
        for (int i = 0; i < nRows(); i++)
            for (int j = 0; j < nColumns(); j++)
                r.modify(i, j, get(i, j) + a);
        return r;
    }
    // scalar multiplication
    Matrix scalarMul(float factor) {
        Matrix r;
        for (int i = 0; i < nRows(); i++)
            for (int j = 0; j < nColumns(); j++)
                r.modify(i, j, get(i, j) * factor);
        return r;
    }
    // matrix addition
    Matrix add(Matrix b) {
        if (nRows() != b.nRows() || nColumns() != b.nColumns()) {
            cout << "The sizes of matrics do not match!" << endl;
        }
        else {
            Matrix r(b.nColumns());
            for (int i = 0; i < nRows(); i++) {
                for (int j = 0; j < nColumns(); j++) {
                    r.modify(i, j, get(i, j) + b.get(i, j));
                }
            }
            return r;
        }
    }
    // matrix multiplication
    Matrix mul(Matrix b) {
        if (nColumns() != b.nRows()) {
            cout << "These two matrics can not be multiplied!\n";
        }
        else {
            float* n = new float[nRows() * b.nColumns()];
            int c = 0;
            for (int i = 0; i < nRows(); i++) {
                for (int j = 0; j < b.nColumns(); j++) {
                    float temp = 0.0;
                    for (int k = 0; k < nColumns(); k++) {
                        temp += get(i, k) * b.get(k, j);
                    }
                    n[c] = temp;
                    c++;
                }
            }
            return Matrix(n, nRows(), b.nColumns());
        }
    }
    // get the sizes of the matrix
    int nRows() {
        return matrix.size();
    }
    int nColumns() {
        return matrix[0].size();
    }
    // get the value of elements in the position (m, n)
    float get(int m, int n) {
        if (m >= nRows() || n >= nColumns()) {
            cout << "The position is out of the matrix!" << endl;
        }
        else {
            return matrix[m][n];
        }

    }
    Vector toVector() {
        if (matrix.size() != 4 || matrix[0].size() != 1) {
            cout << "Cannot convert to Vector!\n";
        }
        else {
            return Vector(matrix[0][0] / matrix[3][0], matrix[1][0] / matrix[3][0], matrix[2][0] / matrix[3][0]);
        }
    }
protected:
    // using vector array for representing the matrix
    vector<vector<float>> matrix;
};