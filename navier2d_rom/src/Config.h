#pragma once
#include <string>

struct Config
{
    // PDE grid
    int Nx, Ny;
    double Lx, Ly;

    // Time
    double dt;
    double finalTime;

    // Physics
    double viscosity;

    // IO
    int snapshotInterval;
    std::string snapshotFile;

    // ROM
    int numPodModes;

    // Read from a plain text file
    static Config fromTXT(const std::string& filename);
};
