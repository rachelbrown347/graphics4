#pragma once

#include <memory>

#include "Vector.h"
#include "Matrix.h"
#include "GenMatrix.h"

void drawLink(Vector, Vector);

struct Link {
    double d; //length
    Vector r; //rotation
    double a; //offset

    std::shared_ptr<Link> child;

    mat r_cross_matrix() const {
        Vector rnorm = (this->r).norm();
        vec rhat = {rnorm[0], rnorm[1], rnorm[2]};
        mat R(3, 3);
        R(0, 0) = 0;
        R(1, 1) = 0;
        R(2, 2) = 0;
        R(0, 1) =  rhat[2];
        R(1, 0) = -rhat[2];
        R(0, 2) =  rhat[1];
        R(2, 0) = -rhat[1];
        R(2, 1) =  rhat[0];
        R(1, 2) = -rhat[0];
        return R;
    }

    mat rotMat() {
        mat I(3, 3);
        I.eye();

        if ((this->r).mag() < 0.0001) {
            return I;
        }
        Vector rnorm = (this->r).norm();
        vec rhat = {rnorm[0], rnorm[1], rnorm[2]};

        double theta = norm(rhat, 2);
        mat rcross = r_cross_matrix();

        return rcross * sin(theta) + I
             + rcross * rcross * (1 - cos(theta));
    }

    Matrix trMat() {
        mat R = rotMat();
        vec p = {this->d, 0, 0};
        vec pi = R * p;
        return {{
            {R(0, 0), R(0, 1), R(0, 2), pi[0]},
            {R(1, 0), R(1, 1), R(1, 2), pi[1]},
            {R(2, 0), R(2, 1), R(2, 2), pi[2]},
            {      0,       0,       0,     1}
        }};
    }

    void getVector(Matrix F = EYE) {
        mat R = rotMat();
        vec p = {this->d, 0, 0};
        vec pi = R * p;
        Matrix T = this->trMat();

        Vector origin = {0, 0, 0, 1};
        Vector endpoint = {pi[0], pi[1], pi[2], 1};

        drawLink(F * origin, F * endpoint);
        if (this->child) {
            this->child->getVector(F * T);
        }
    }

    Vector getEndpoint(Matrix F = EYE) {
        Vector endpoint = {this->d, 0, 0, 1};

        Matrix T = this->trMat();
        
        if (this->child) {
            return this->child->getEndpoint(F * T);
        } else {
            return F * T * endpoint;
        }
    }

    Vector getCostDiff(Vector goal) {
        Vector endpoint = this->getEndpoint();
        return endpoint - goal;
    }

    std::vector<Link*> getLinks() {
        std::vector<Link*> links;
        Link* head = this;
        while (head) {
            links.push_back(head);
            head = head->child.get();
        }
        return links;
    }

    std::vector<Vector> calcJacobianVectors() {
        std::vector<Link*> links = this->getLinks();
        std::vector<Vector> jVectors;
        double e = 0.0001;
        for (Link* link : links) {
            for (size_t i = 0; i < 3; i++) {
                double orig = (*link).r[i];

                (*link).r[i] = orig + e;
                Vector vPlus = this->getEndpoint();

                (*link).r[i] = orig - 2 * e;
                Vector vMinus = this->getEndpoint();

                jVectors.push_back((vPlus - vMinus) / 2.0 * e);
                (*link).r[i] = orig;
            }
        }
        return jVectors;
    }

    // std::vector<Link*> getLinks() {
    //     std::vector<Link*> links;
    //     Link* head = this;
    //     while (head) {
    //         links.push_back(head);
    //         head = head->child.get();
    //     }
    //     return links;
    // }

    // std::vector<Vector> buildJacobian() {
    //     std::vector<Link*> links = this->getLinks();
    //     size_t links.size() = numLinks;
    //     mat jVectors(3, 3*numLinks);
    //     for (i = 0; i < numLinks; i++) {
    //         Link* link = links[i];
            
    //         mat X_ni(4, 4);
    //         X_ni.eye();

    //         for (j = i; j < numLinks; j++) {
    //             X_ni = links[j].trMat() * X_ni;
    //         }

    //         mat R_io(3, 3);
    //         R_io.eye();

    //         for (k = 1; k < i; k++) {
    //             R_io = links[k].rotMat() * R_io;
    //         }

    //         //mat jPrime =
            
    //     }
    //     return jVectors;
    // }


    // Vector getEndpoint(Matrix F = EYE) {
    //     Vector endpoint = {1, 0, 0, 1};

    //     Matrix T = this->T();
        
    //     if (this->child) {
    //         return this->child->getEndpoint(F * T);
    //     } else {
    //         return F * T * endpoint;
    //     }
    // }

    // Vector getCostDiff(Vector goal) {
    //     Vector endpoint = this->getEndpoint();
    //     return endpoint - goal;
    // }

        // double e = 0.0001;
        // for (Link* link : links) {
        //     for (size_t i = 0; i < 5; i++) {
        //         double orig = (*link)[i];

        //         (*link)[i] = orig + e;
        //         Vector vPlus = this->getEndpoint();

        //         (*link)[i] = orig - 2 * e;
        //         Vector vMinus = this->getEndpoint();

        //         jVectors.push_back((vPlus - vMinus) / 2.0 * e);
        //         (*link)[i] = orig;
        //     }
        // }
};

