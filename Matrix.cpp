#include"pch.h"
#include"Matrix.h"
#include"Vector.h"


// create an all-zero matrix of size 4 as default construction
Matrix::Matrix() {
    matrix.resize(4);
    for (int i = 0; i < 4; i++) {
        // create a row of 4 element with the value 0
        vector<float> temp(4, 0);
        // add the row to the matrix
        matrix[i] = temp;
    }
}

// create an identity matrix of size n
Matrix::Matrix(int n) {
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
Matrix::Matrix(Vector v) {
    matrix.resize(4);
    for (int i = 0; i < 4; i++) {
        matrix[i].resize(1);
    }
    matrix[0][0] = v.x;
    matrix[1][0] = v.y;
    matrix[2][0] = v.z;
    matrix[3][0] = 1;
}

// create a all-zero matrix of m*n
Matrix::Matrix(int m, int n) {
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
Matrix::Matrix(float A[], int m, int n) {
    matrix.resize(m);
    for (int i = 0; i < m; i++) {
        // create a row of n element and assign the values from array A[]
        vector<float> temp(A + i * n, A + (i + 1) * n);
        // add the row to the matrix
        matrix[i] = temp;
    }
}

Matrix::~Matrix() {
    vector<vector<float>>().swap(matrix);
}

// modify the matrix value in position (m, n) with the new value a
void Matrix::modify(int m, int n, float a) {
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
Matrix Matrix::scalarAdd(float a) {
    Matrix r;
    for (int i = 0; i < nRows(); i++)
        for (int j = 0; j < nColumns(); j++)
            r.modify(i, j, get(i, j) + a);
    return r;
}

// scalar multiplication
Matrix Matrix::scalarMul(float factor) {
    Matrix r;
    for (int i = 0; i < nRows(); i++)
        for (int j = 0; j < nColumns(); j++)
            r.modify(i, j, get(i, j) * factor);
    return r;
}

// matrix addition
Matrix Matrix::add(Matrix b) {
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
Matrix Matrix::mul(Matrix b) {
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
int Matrix::nRows() {
    return matrix.size();
}

int Matrix::nColumns() {
    return matrix[0].size();
}

// get the value of elements in the position (m, n)
float Matrix::get(int m, int n) {
    if (m >= nRows() || n >= nColumns()) {
        cout << "The position is out of the matrix!" << endl;
    }
    else {
        return matrix[m][n];
    }

}

// convert 1D-Matrix to Vector
// modify x,y value so that they coordinate with screen
// x:[0,width]
// y:[0,height]
Vector Matrix::fitToScreen(int width, int height) {
    assert(matrix.size() == 4 && matrix[0].size() == 1);
    float x = ((matrix[0][0] / matrix[3][0]) + 1.0) * (width / 2.0);
    float y = ((matrix[1][0] / matrix[3][0]) + 1.0) * (height / 2.0);
    float z = matrix[2][0] / matrix[3][0];
    return Vector(x, y, z);
}