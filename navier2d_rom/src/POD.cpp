#include "POD.h"
#include <Eigen/SVD>
#include <iostream>

POD::POD(int numModes)
    : k_(numModes)
{}

void POD::computeBasis(const Eigen::MatrixXd& X) {
    // X is n x m
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(
        X, Eigen::ComputeThinU | Eigen::ComputeThinV
    );
    const auto& U = svd.matrixU();
    const auto& S = svd.singularValues();
    int rank = std::min<int>(S.size(), k_);
    basis_ = U.leftCols(rank).eval();
    std::cout << "[POD] Computed basis with rank="<<rank<<"\n";
}
