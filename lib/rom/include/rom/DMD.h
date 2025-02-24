#pragma once

#include "ReducedOrderModel.h"

class DMD : public ReducedOrderModel {
public:
    DMD();
    void train(const Eigen::MatrixXd& X) override;
    Eigen::VectorXd predict(const Eigen::VectorXd& input) override;

private:
    Eigen::MatrixXd A_; // The learned linear operator
};
