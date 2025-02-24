#pragma once

#include "ReducedOrderModel.h"

class POD : public ReducedOrderModel {
public:
    POD(int numModes);
    virtual ~POD() {}

    void train(const Eigen::MatrixXd& X) override;
    Eigen::VectorXd predict(const Eigen::VectorXd& input) override;

    // Expose the POD basis in case other classes (GalerkinROM, DEIM) need it
    const Eigen::MatrixXd& getBasis() const { return basis_; }

protected:
    int k_;                        // number of modes
    Eigen::MatrixXd basis_;        // n x k
    Eigen::VectorXd singularVals_; // k x 1
};

