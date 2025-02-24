#include <iostream>
#include <fstream>
#include <Eigen/Dense>

#include "rom/ReducedOrderModel.h"
#include "rom/POD.h"
#include "rom/GalerkinROM.h"
#include "rom/DEIM.h"
#include "rom/DMD.h"

// Helper: Load matrix from file
Eigen::MatrixXd loadMatrix(const std::string& filename) {
    // Format:
    //  n m
    //  x11 x12 ... x1m
    //  ...
    //  xn1 xn2 ... xnm
    std::ifstream file(filename);
    if(!file.is_open()) {
        throw std::runtime_error("Failed to open " + filename);
    }

    int n, m;
    file >> n >> m;
    Eigen::MatrixXd X(n, m);

    for(int i=0; i<n; ++i) {
        for(int j=0; j<m; ++j) {
            double val;
            file >> val;
            X(i,j) = val;
        }
    }
    return X;
}

// Helper: write vector to file
void writeVector(const std::string& fname, const Eigen::VectorXd& vec) {
    std::ofstream ofs(fname);
    if(!ofs.is_open()) {
        std::cerr << "Cannot open " << fname << " for writing.\n";
        return;
    }
    for(int i=0; i<vec.size(); ++i) {
        ofs << vec(i) << "\n";
    }
    ofs.close();
}

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] 
                  << " <snapshot_file> [numModes=5]" << std::endl;
        return 1;
    }

    // 1) Parse args
    std::string fname = argv[1];
    int numModes = (argc > 2) ? std::stoi(argv[2]) : 5;

    // 2) Load snapshots
    Eigen::MatrixXd X = loadMatrix(fname);
    std::cout << "Loaded X: " << X.rows() << " x " << X.cols() << std::endl;

    // Use first snapshot as an example "initial condition"
    Eigen::VectorXd initialCond = X.col(0);

    // 3) Instantiate ROMs
    POD podROM(numModes);
    GalerkinROM galerkinROM(numModes);
    // For DEIM, choose a separate number of "nonlinear modes" e.g., 3
    DEIM deimROM(numModes, 3);
    DMD dmdROM;

    // 4) Train
    podROM.train(X);
    galerkinROM.train(X);
    deimROM.train(X);
    dmdROM.train(X);

    // 5) Predict
    Eigen::VectorXd podPredict       = podROM.predict(initialCond);
    Eigen::VectorXd galerkinPredict  = galerkinROM.predict(initialCond);
    Eigen::VectorXd deimPredict      = deimROM.predict(initialCond);
    Eigen::VectorXd dmdPredict       = dmdROM.predict(initialCond);

    // 6) Write results to files
    writeVector("results_pod.txt", podPredict);
    writeVector("results_galerkin.txt", galerkinPredict);
    writeVector("results_deim.txt", deimPredict);
    writeVector("results_dmd.txt", dmdPredict);

    // Maybe also write the initial condition for reference
    writeVector("results_initial.txt", initialCond);

    std::cout << "Done. Plot 'results_*.txt' for comparison.\n";

    return 0;
}
