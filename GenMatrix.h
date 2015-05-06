#pragma once

#include <armadillo>
using namespace arma;

mat getPseudoInv(std::vector<Vector> jVectors) {
    int n = jVectors.size();
    mat J(3, n);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < 3; j++) {
            J(j, i) = jVectors[i][j];
        }
    }

    // SVD Inverse Method
    mat U;
    vec s;
    mat V;

    svd(U, s, V, J);
    int num = s.n_elem;
    for (size_t i = 0; i < num; i++) {
        if (s[i] < 0.0001) {
            s[i] = 0;
        } else {
            s[i] = 1.0 / s[i];
        }
    }

    mat littleS = diagmat(s);
    mat bigZeros = zeros(n - 3, 3);
    mat bigS = join_vert(littleS, bigZeros);

    mat jOut = V * bigS * U.t();


    // // Pseudo Inverse Method
    // mat jT = J.t();
    // mat jTj = jT * J;
    // mat jTInv;
    // bool invertible = inv(jTInv, jTj);
    // if (!invertible) {
    //     jTInv = jTj;
    //     inplace_trans(jTInv);
    //     for (size_t i = 0; i < n; i++) {
    //         // change into 1/value
    //         // if smaller than 0.0001 than just zero
    //         jTInv(i, i) *= -1;
    //     }
    // }
    // mat jOut = jTInv * jT;

    return jOut;
}

