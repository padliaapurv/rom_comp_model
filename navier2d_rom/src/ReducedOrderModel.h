#pragma once
#include <Eigen/Dense>
#include <string>

class ReducedOrderModel {
public:
    virtual ~ReducedOrderModel() {}

    // Train on snapshot matrix X
    virtual void train(const Eigen::MatrixXd& X) = 0;

    // Predict or reconstruct from an initial condition
    virtual Eigen::VectorXd predict(const Eigen::VectorXd& initialState) = 0;

    // For reporting or saving results
    virtual void saveResults(const std::string& filename) {}
};
