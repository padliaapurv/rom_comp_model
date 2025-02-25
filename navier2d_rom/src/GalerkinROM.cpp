#include "GalerkinROM.h"
#include <cmath>
#include <iostream>

GalerkinROM::GalerkinROM(const POD& pod, int Nx, int Ny, double dx, double dy, double nu)
    : pod_(pod), Nx_(Nx), Ny_(Ny), dx_(dx), dy_(dy), nu_(nu)
{
    n_ = 2*Nx_*Ny_; // we store [u, v]
}

// Reconstruct from a => \Phi a
Eigen::VectorXd GalerkinROM::reconstructFull(const Eigen::VectorXd& a) {
    return pod_.basis() * a;
}

// PDE residual in full dimension
// R(u) = - (u dot grad)u + nu lap(u)
Eigen::VectorXd GalerkinROM::computeResidual(const Eigen::VectorXd& uFull) {
    // uFull(0..Nx_*Ny_-1) => U
    // uFull(Nx_*Ny_..2*Nx_*Ny_-1) => V
    // We'll produce R(0..Nx_*Ny_-1) for dU/dt, R(Nx_*Ny_..end) for dV/dt

    Eigen::VectorXd R = Eigen::VectorXd::Zero(n_);

    auto idx = [&](int i, int j){ return i + j*Nx_; };

    // Convert to 2D arrays for easier stencils
    // but let's do it in-situ
    // Let U(i,j) = uFull( idx(i,j) ),  V(i,j)=uFull( idx(i,j)+Nx_*Ny_ )

    // Central differences for gradients
    for(int j=1; j<Ny_-1; j++){
        for(int i=1; i<Nx_-1; i++){
            int base = idx(i,j);
            double uij = uFull(base);
            double vij = uFull(base + Nx_*Ny_);

            // partial derivatives
            auto dudx = (uFull(base+1)     - uFull(base-1))     / (2*dx_);
            auto dudy = (uFull(base+Nx_)   - uFull(base-Nx_))   / (2*dy_);
            auto dvdx = (uFull(base+1+Nx_*Ny_) - uFull(base-1+Nx_*Ny_)) / (2*dx_);
            auto dvdy = (uFull(base+Nx_+Nx_*Ny_) - uFull(base-Nx_+Nx_*Ny_)) / (2*dy_);

            // convection
            double convU = uij*dudx + vij*dudy;
            double convV = uij*dvdx + vij*dvdy;

            // Laplacian
            auto lap = [&](auto indexOffset){
                double left  = uFull(base-1+indexOffset);
                double right = uFull(base+1+indexOffset);
                double up    = uFull(base+Nx_+indexOffset);
                double down  = uFull(base-Nx_+indexOffset);
                double cent  = uFull(base+indexOffset);
                double d2x = (left - 2*cent + right)/(dx_*dx_);
                double d2y = (down - 2*cent + up)/(dy_*dy_);
                return d2x + d2y;
            };

            double lapU = lap(0);
            double lapV = lap(Nx_*Ny_);

            R(base)              = -convU + nu_*lapU;
            R(base+Nx_*Ny_)      = -convV + nu_*lapV;
        }
    }

    // zero boundary
    // for simplicity
    // you could do better BC approximations
    return R;
}

// compute \Phi^T * R(\Phi a)
Eigen::VectorXd GalerkinROM::computeReducedRHS(const Eigen::VectorXd& a) {
    Eigen::VectorXd uFull = reconstructFull(a);
    Eigen::VectorXd Rfull = computeResidual(uFull);
    // project
    return pod_.basis().transpose() * Rfull;
}

Eigen::VectorXd GalerkinROM::stepExplicitEuler(const Eigen::VectorXd& a, double dt) {
    Eigen::VectorXd rhs = computeReducedRHS(a);
    return a + dt*rhs;
}

const POD& GalerkinROM::pod() const {
    return pod_;
}
