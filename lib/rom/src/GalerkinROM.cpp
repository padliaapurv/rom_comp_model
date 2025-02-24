#include "rom/GalerkinROM.h"
#include <iostream>

GalerkinROM::GalerkinROM(int numModes)
    : POD(numModes)
{}

void GalerkinROM::train(const Eigen::MatrixXd& X) {
    // 1) We still want to do POD on the snapshots:
    POD::train(X);

    // 2) Possibly do other tasks if needed
    std::cout << "[GalerkinROM::train] Completed POD basis for Galerkin.\n";
}

// A simple function f(u) = -u (dummy linear operator)
Eigen::VectorXd GalerkinROM::f(const Eigen::VectorXd& u) {
    return -u; 
}

// We do a very simplified time stepping for illustration
Eigen::VectorXd GalerkinROM::predict(const Eigen::VectorXd& input) {
    // 1) Project initial condition
    Eigen::VectorXd a = basis_.transpose() * input;  // (k x 1)

    // 2) Let's do a few Euler steps: da/dt = Phi^T f(Phi a)
    double dt = 0.01;
    int numSteps = 100;  // arbitrary
    for(int step = 0; step < numSteps; ++step) {
        // Evaluate PDE in reduced coords
        Eigen::VectorXd fullState = basis_ * a;           // n x 1
        Eigen::VectorXd rhs = basis_.transpose() * f(fullState); // k x 1

        // Forward Euler: a_{n+1} = a_n + dt * rhs
        a = a + dt * rhs;
    }

    // 3) Reconstruct
    Eigen::VectorXd finalState = basis_ * a;
    return finalState;
}
