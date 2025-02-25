#pragma once
#include <Eigen/Dense>
#include <vector>
#include "POD.h"

class GalerkinROM {
public:
    // constructor: pass the POD basis, domain sizes, etc. 
    GalerkinROM(const POD& pod, int Nx, int Ny, double dx, double dy, double nu);

    // returns da/dt for a given a(t) in the reduced space
    // i.e. \Phi^T * R(\Phi a(t))
    Eigen::VectorXd computeReducedRHS(const Eigen::VectorXd& a);

    // build a function to do one step: a_{n+1} = a_n + dt * RHS
    // (explicit Euler for demonstration)
    Eigen::VectorXd stepExplicitEuler(const Eigen::VectorXd& a, double dt);

    const POD& pod() const;

private:
    const POD& pod_;
    int Nx_, Ny_;
    double dx_, dy_, nu_;
    int n_; // 2*Nx_*Ny_ for storing (u,v)

    // Reconstruct full vector from a
    Eigen::VectorXd reconstructFull(const Eigen::VectorXd& a);

    // PDE residual in full dimension: R(uFull) => dimension n_
    Eigen::VectorXd computeResidual(const Eigen::VectorXd& uFull);
};
