#include "rom/DMD.h"
#include <Eigen/SVD>
#include <iostream>

DMD::DMD() {}

void DMD::train(const Eigen::MatrixXd& X) {
    // X is n x m, assume columns are snapshots: X=[u_1, ..., u_m].
    // We'll form X1=[u_1,...,u_{m-1}] and X2=[u_2,...,u_m].
    int n = X.rows();
    int m = X.cols();
    if(m < 2) {
        std::cerr << "[DMD::train] Need at least 2 snapshots.\n";
        return;
    }

    Eigen::MatrixXd X1 = X.leftCols(m-1);  // n x (m-1)
    Eigen::MatrixXd X2 = X.rightCols(m-1); // n x (m-1)

    // Pseudoinverse approach: A = X2 * pinv(X1)
    // We'll use Eigen's completeOrthogonalDecomposition.
    Eigen::CompleteOrthogonalDecomposition<Eigen::MatrixXd> cod(X1);
    A_ = X2 * cod.pseudoInverse();

    std::cout << "[DMD::train] DMD matrix A_ is " 
              << A_.rows() << "x" << A_.cols() << std::endl;
}

Eigen::VectorXd DMD::predict(const Eigen::VectorXd& input) {
    // next state = A_ * input
    return A_ * input;
}
