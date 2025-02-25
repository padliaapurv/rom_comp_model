#pragma once
#include <vector>
#include <Eigen/Dense>
#include <string>
#include "Config.h"

// Offline solver for 2D Burgers: solves in full dimension
// and writes snapshots to file.
class OfflineSolver2D {
public:
    explicit OfflineSolver2D(const Config& cfg);

    // Run the PDE solve and write snapshots
    void runOfflineSolve();

private:
    Config cfg_;

    int Nx_, Ny_;
    double Lx_, Ly_, dx_, dy_;
    double dt_, finalTime_;
    double nu_; // viscosity
    int snapshotInterval_;
    std::string snapshotFile_;

    // Full-state storage: each cell has (u, v)
    // We'll store them in 1D arrays of size Nx_*Ny_
    std::vector<double> u_, v_;

    // Temporary arrays for next step
    std::vector<double> uNext_, vNext_;

    // We'll keep snapshots in memory, then write at the end
    std::vector<Eigen::VectorXd> snapshots_;

    // Helpers
    void initialize();
    void stepExplicit();
    void storeSnapshot(double time);
    void writeSnapshotsToFile();
    inline int idx(int i, int j) const { return i + j*Nx_; }
};
