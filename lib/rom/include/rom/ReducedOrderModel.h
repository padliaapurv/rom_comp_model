#pragma once

#include <Eigen/Dense>

class ReducedOrderModel {
public:
    // Train the ROM on a snapshot matrix X
    virtual void train(const Eigen::MatrixXd& X) = 0;

    // Predict or reconstruct from an input state (or amplitude vector)
    virtual Eigen::VectorXd predict(const Eigen::VectorXd& input) = 0;

    // Optional: write final results or states for plotting
    virtual void saveResults(const std::string& filename) {}

    virtual ~ReducedOrderModel() {}
};
