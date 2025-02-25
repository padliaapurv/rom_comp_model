#include "OnlineSolver2D.h"
#include <cmath>
#include <iostream>
#include "POD.h"

OnlineSolver2D::OnlineSolver2D(const Config& cfg, GalerkinROM& rom)
    : cfg_(cfg), rom_(rom)
{
    dt_ = cfg_.dt;
    finalTime_ = cfg_.finalTime;
}

Eigen::VectorXd OnlineSolver2D::toReduced(const Eigen::VectorXd& x) {
    // a = Phi^T x
    const Eigen::MatrixXd& Phi = rom_.pod().basis(); // we'll adjust to get that
    return Phi.transpose() * x;
}

Eigen::VectorXd OnlineSolver2D::toFull(const Eigen::VectorXd& a) {
    const Eigen::MatrixXd& Phi = rom_.pod().basis();
    return Phi * a;
}

Eigen::VectorXd OnlineSolver2D::runReducedSolve(const Eigen::VectorXd& initialFull) {
    // 1) convert to reduced
    Eigen::VectorXd a = toReduced(initialFull);

    int steps = static_cast<int>(std::ceil(finalTime_/dt_));
    for(int s=0; s<steps; s++){
        a = rom_.stepExplicitEuler(a, dt_);
    }

    // 2) reconstruct final
    Eigen::VectorXd finalFull = toFull(a);
    return finalFull;
}
