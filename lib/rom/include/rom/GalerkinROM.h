#pragma once

#include "POD.h"

class GalerkinROM : public POD {
public:
    // constructor: pass number of modes, and maybe PDE parameters
    GalerkinROM(int numModes);

    void train(const Eigen::MatrixXd& X) override;

    // We'll do a time-stepping approach for "predict" to mimic PDE solution
    // input: initial full state
    Eigen::VectorXd predict(const Eigen::VectorXd& input) override;

protected:
    // For a real PDE, you'd store PDE data, boundary conditions, etc.
    // We'll define a simple "F(u)" = -u, just to illustrate
    virtual Eigen::VectorXd f(const Eigen::VectorXd& u);
};
