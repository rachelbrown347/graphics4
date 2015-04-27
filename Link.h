#pragma once

#include <memory>

#include "Vector.h"
#include "Matrix.h"
#include "GenMatrix.h"

void drawLink(Vector, Vector);

struct Link {
    double d; //length
    double theta; //azimuth
    double alpha; //elevation
    double a; //offset

    std::shared_ptr<Link> child;

    double& operator[](const size_t i) {
        switch (i) {
        case 0:
            return d;
        case 1:
            return theta;
        case 2:
            return alpha;
        case 3:
            return a;
        default:
            assert(0);
        }
    }

    double operator[](const size_t i) const {
        switch (i) {
        case 0:
            return d;
        case 1:
            return theta;
        case 2:
            return alpha;
        case 3:
            return a;
        default:
            assert(0);
        }
    }


    Matrix T() const {
        return {{
            {cos(theta), -sin(theta) * cos(alpha),  sin(theta) * sin(alpha), a * cos(theta)},
            {sin(theta),  cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta)},
            {         0,               sin(alpha),               cos(alpha),              d},
            {         0,                        0,                        0,              1}
        
        }};
    }

    void getVector(Matrix F = EYE) {
        Vector origin = {0, 0, 0, 1};
        Vector endpoint = {0, 0, 1, 1};

        Matrix T = this->T();
        
        drawLink(F * T * origin, F * T * endpoint);
        if (this->child) {
            this->child->getVector(F * T);
        }
    }

    Vector getEndpoint(Matrix F = EYE) {
        Vector endpoint = {0, 0, 1, 1};

        Matrix T = this->T();
        
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
            for (size_t i = 0; i < 5; i++) {
                double orig = (*link)[i];

                (*link)[i] = orig + e;
                Vector vPlus = this->getEndpoint();

                (*link)[i] = orig - 2 * e;
                Vector vMinus = this->getEndpoint();

                jVectors.push_back((vPlus - vMinus) / 2.0 * e);
                (*link)[i] = orig;
            }
        }
        return jVectors;
    }

    std::vector<Vector> getNewParams(std::vector<Vector> jVectors) {
        
    }
};

