#pragma once
#include <Eigen/Dense>
#include <vector>
#include <string>
#include "Config.h"
#include "GalerkinROM.h"

class OnlineSolver2D {
public:
    OnlineSolver2D(const Config& cfg, GalerkinROM& rom);

    // Run the reduced solve from an initial condition a0
    // returns final solution in FULL space for comparison
    Eigen::VectorXd runReducedSolve(const Eigen::VectorXd& initialFull);

private:
    Config cfg_;
    GalerkinROM& rom_;

    double dt_, finalTime_;

    // Convert from full initial vector to reduced coords: a0 = Phi^T * x0
    Eigen::VectorXd toReduced(const Eigen::VectorXd& x);
    // Reconstruct from a => x
    Eigen::VectorXd toFull(const Eigen::VectorXd& a);
};
