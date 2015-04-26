#pragma once

#include <cmath>
#include <cassert>

struct Matrix {
    double A[4][4];

    bool operator==(const Matrix& B) const {
        for (size_t i=0; i < 4; i++) {
            for (size_t j=0; j < 4; j++) {
                if (B(i, j) != (*this)(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    double& operator()(const size_t i, const size_t j) {
        return this->A[i][j];
    }

    double operator()(const size_t i, const size_t j) const {
        return this->A[i][j];
    }

    Matrix operator+(const Matrix& B) {
        Matrix C = {};
        for (size_t i=0; i < 4; i++) {
            for (size_t j=0; j < 4; j++) {
                C(i, j) = (*this)(i, j) + B(i, j);
            }
        }
        return C;
    }

    Matrix operator*(const float& b) const {
        Matrix C = {};
        for (size_t i=0; i < 4; i++) {
            for (size_t j=0; j < 4; j++) {
                C(i, j) = (*this)(i, j) * b;
            }
        }
        return C;
    }

    Matrix operator*(const Matrix& B) const {
        Matrix C = {};
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                for (size_t k = 0; k < 4; k++) {
                    C(i, j) += (*this)(i, k) * B(k, j);
                }
            }
        }
        assert(C(3, 3) == 1);
        return C;
    }
    
    Vector operator*(const Vector& b) const {
        Vector bp = b;
        bp.w = 1.;
        Vector c = {};
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                c[i] += (*this)(i, j) * bp[j];
            }
        }
        return c;
    }
};

Matrix EYE = {{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
}};

Matrix rotX(double theta) {
    return {{
        {1,          0,           0, 0},
        {0, cos(theta), -sin(theta), 0},
        {0, sin(theta),  cos(theta), 0},
        {0,          0,           0, 1}
    }};
}

Matrix rotY(double theta) {
    return {{
        { cos(theta), 0, sin(theta), 0},
        {          0, 1,          0, 0},
        {-sin(theta), 0, cos(theta), 0},
        {          0, 0,          0, 1}
    }};
}

Matrix rotZ(double theta) {
    return {{
        {cos(theta), -sin(theta), 0, 0},
        {sin(theta),  cos(theta), 0, 0},
        {         0,           0, 1, 0},
        {         0,           0, 0, 1}
    }};
}

Matrix transX(double tx) {
    return {{
        {1, 0, 0, tx},
        {0, 1, 0,  0},
        {0, 0, 1,  0},
        {0, 0, 0,  1}
    }};
}

Matrix transY(double ty) {
    return {{
        {1, 0, 0,  0},
        {0, 1, 0, ty},
        {0, 0, 1,  0},
        {0, 0, 0,  1}
    }};
}

Matrix transZ(double tz) {
    return {{
        {tz, 0, 0,  0},
        {0, tz, 0,  0},
        {0, 0, tz, 0},
        {0, 0, 0,  1}
    }};
}


