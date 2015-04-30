#pragma once

#include <armadillo>
using namespace arma;

mat getPseudoInv(std::vector<Vector> jVectors) {
    int n = jVectors.size();
    mat J(3, n);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < 4; j++) {
            J(i, j) = jVectors[i][j];
        }
    }

    mat jT = J.t();
    mat jTInv = inv(jT * J);
    mat jOut = jTInv * jT;

    return jOut;
}