#pragma once

#include <memory>

#include "Vector.h"
#include "Matrix.h"
#include "GenMatrix.h"

void drawLink(Vector, Vector);

struct Link {
    const double d; //length
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
        R(0, 1) = -rhat[2];
        R(1, 0) =  rhat[2];
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

        double theta = (this->r).mag();
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
        mat R = rotMat();
        vec p = {this->d, 0, 0};
        vec pi = R * p;
        Matrix T = this->trMat();
        Vector endpoint = {pi[0], pi[1], pi[2], 1};
        
        if (this->child) {
            return this->child->getEndpoint(F * T);
        } else {
            return F * endpoint;
        }
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

    std::vector<Vector> calcJacobianVectors(double e) {
        std::vector<Link*> links = this->getLinks();
        std::vector<Vector> jVectors;
        for (Link* link : links) {
            for (size_t i = 0; i < 3; i++) {
                double orig = (*link).r[i];

                (*link).r[i] = orig + e;
                Vector vPlus = this->getEndpoint();

                (*link).r[i] = orig - e;
                Vector vMinus = this->getEndpoint();

                jVectors.push_back((vPlus - vMinus) / (2.0 * e));
                (*link).r[i] = orig;
            }
        }
        return jVectors;
    }

    vec getCostDiff(Vector goal) {
//		cout << "getCostDiff:" << goal.x << ',' << goal.y << ',' << goal.z << endl;
		
        Vector endpoint = this->getEndpoint();
		//cout << "Endpoint: " << endpoint.x << ',' << endpoint.y << ',' << endpoint.z << endl;
        Vector diff = endpoint - goal;
		//cout << "diff:" << diff.x << ',' << diff .y << ',' << diff.z << endl;
		
        vec cost = {diff[0], diff[1], diff[2]};
        return cost;
    }

    vec getParams() {
        std::vector<Link*> links = getLinks();
        vec params(3 * links.size());
        for (size_t i = 0; i < links.size(); i++) {
            params[3*i    ] = links[i]->r[0];
            params[3*i + 1] = links[i]->r[1];
            params[3*i + 2] = links[i]->r[2];
        }
        return params;
    }

    void setParams(vec params) {
        std::vector<Link*> links = getLinks();
        for (size_t i = 0; i < links.size(); i++) {
            links[i]->r[0] = params[3*i    ];
            links[i]->r[1] = params[3*i + 1];
            links[i]->r[2] = params[3*i + 2];
        }
    }

    void updateParams(Vector goal, const double step=1.0) {
		//cout << "Params passed:" << goal.x << ',' << goal.y << ',' << goal.z << endl;
        std::vector<Vector> jVectors = calcJacobianVectors(0.001);
        mat pInv = getPseudoInv(jVectors);

        vec currParams = getParams();
        vec currCost = getCostDiff(goal);
        double currError = norm(currCost, 2);
		//cout << "CurrError:" << currError << endl;
		

        vec nextParams = currParams - (pInv * currCost) * step;
        setParams(nextParams);
        vec nextCost = getCostDiff(goal);
        double nextError = norm(nextCost, 2);

        setParams(currParams);

        if (nextError > currError) {
            if (step > 0.01) {
                // try a smaller step
                updateParams(goal, step / 2.0);
            }
        } else {
            // great! take another step of the same size
            setParams(nextParams);
            updateParams(goal);
        }
    }
};

