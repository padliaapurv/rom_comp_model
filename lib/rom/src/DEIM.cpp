#include "rom/DEIM.h"
#include <Eigen/SVD>
#include <iostream>

DEIM::DEIM(int numModes, int numNonlinModes)
    : GalerkinROM(numModes), kf_(numNonlinModes)
{}

void DEIM::train(const Eigen::MatrixXd& X) {
    // 1) First do normal POD / Galerkin basis
    GalerkinROM::train(X);

    // 2) Then do DEIM on the "nonlinear snapshots"
    //    For a real PDE: you'd store "nonlinear snapshots" of F(u_i).
    //    We'll just re-use X as if it was N(u_i). For demonstration only.
    trainDEIM(X);

    std::cout << "[DEIM::train] DEIM completed.\n";
}

void DEIM::trainDEIM(const Eigen::MatrixXd& X) {
    // X is n x m. We'll do a POD on X to get "DEIM basis" Uf_
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(
        X, Eigen::ComputeThinU | Eigen::ComputeThinV
    );
    const Eigen::MatrixXd U = svd.matrixU();
    const Eigen::VectorXd S = svd.singularValues();

    int rank = std::min<int>(S.size(), kf_);
    Uf_ = U.leftCols(rank);

    // For interpolation points, we'll do a trivial choice:
    // Just pick idx_ = [0,1,...,kf_-1]. 
    idx_.resize(rank);
    for(int i=0; i<rank; ++i) {
        idx_(i) = i;  // obviously not a real pivot-based selection
    }
}

Eigen::VectorXd DEIM::f(const Eigen::VectorXd& u) {
    // Suppose our PDE has a nonlinear term N(u). 
    // For demonstration, let's do N(u) = "elementwise square" -> u^2
    // Then approximate via DEIM.

    // Step A: compute N(u) in the full dimension (for demonstration).
    // In a real code, you'd do partial sampling. We'll keep it simple:
    Eigen::VectorXd N_u = u.array().square().matrix(); // elementwise square

    // Step B: DEIM approximation: 
    // N(u) ~ Uf_ * ( (P^T Uf_)^-1 * P^T * N(u) ), 
    // where P is the selection matrix of size (n x kf_).
    // We'll build P on the fly:
    int n = Uf_.rows();
    int kf = Uf_.cols();

    // Build P
    Eigen::MatrixXd P = Eigen::MatrixXd::Zero(n, kf);
    for(int i=0; i<kf; ++i) {
        P(idx_(i), i) = 1.0;
    }

    // Then approximate:
    // intermediate = (P^T Uf_)^-1 * P^T * N(u)
    Eigen::MatrixXd PTUf = P.transpose() * Uf_; // (kf x kf)
    Eigen::VectorXd PTNu = P.transpose() * N_u; // (kf x 1)

    Eigen::VectorXd alpha = PTUf.colPivHouseholderQr().solve(PTNu);
    Eigen::VectorXd N_approx = Uf_ * alpha; // (n x 1)

    return N_approx;
}
