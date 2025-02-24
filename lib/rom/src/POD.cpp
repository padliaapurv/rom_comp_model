#include "rom/POD.h"
#include <Eigen/SVD>
#include <iostream>

POD::POD(int numModes) : k_(numModes) {}

void POD::train(const Eigen::MatrixXd& X) {
    // X is n x m (n=dimension, m=#snapshots)
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(
        X, Eigen::ComputeThinU | Eigen::ComputeThinV
    );

    // U: n x r, S: r x r, V: m x r (r=rank)
    const Eigen::MatrixXd U = svd.matrixU();
    const Eigen::VectorXd S = svd.singularValues();

    int rank = std::min<int>(S.size(), k_);  // clamp to rank or k_
    basis_ = U.leftCols(rank);
    singularVals_ = S.head(rank);

    std::cout << "[POD::train] Kept " << rank << " modes." << std::endl;
}

Eigen::VectorXd POD::predict(const Eigen::VectorXd& input) {
    // Project input onto basis (k-dim)
    Eigen::VectorXd a = basis_.transpose() * input; // (k x 1)
    // Reconstruct
    Eigen::VectorXd reconstructed = basis_ * a;     // (n x 1)
    return reconstructed;
}
