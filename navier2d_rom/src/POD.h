#pragma once
#include <Eigen/Dense>

class POD {
public:
    POD(int numModes);

    // Compute basis from snapshots X (n x m)
    void computeBasis(const Eigen::MatrixXd& X);

    // Return the POD basis matrix (n x k)
    const Eigen::MatrixXd& basis() const { return basis_; }

private:
    int k_;
    Eigen::MatrixXd basis_;  // n x k
};
