#pragma once

#include "GalerkinROM.h"

class DEIM : public GalerkinROM {
public:
    DEIM(int numModes, int numNonlinModes);

    void train(const Eigen::MatrixXd& X) override;

protected:
    int kf_; // number of DEIM modes
    Eigen::MatrixXd Uf_; // DEIM basis
    Eigen::VectorXi idx_; // interpolation indices

    // Overridden PDE operator that uses DEIM approximation
    Eigen::VectorXd f(const Eigen::VectorXd& u) override;

private:
    void trainDEIM(const Eigen::MatrixXd& X);
};
