#pragma once

#include <armadillo>
using namespace arma;

mat getPseudoInv(std::vector<Vector> jVectors) {
    int n = jVectors.size;
    mat J(3, n);

    for (size_t i = 0; i < n; i++) {
        J.col(i) = jVectors[i];
    }

    jT = J.t();
    jTInv = inv(jT * J);
    jOut = jTInv * jT;

    return jOut
}