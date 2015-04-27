#pragma once

#include <armadillo>
using namespace arma;

mat buildJacobian(std::vector<Vector> jVectors) {
    int n = jVectors.size;
    mat J(3, n);

    for (size_t i = 0; i < n; i++) {
        J.col(i) = jVectors[i];
    }

    return J
}