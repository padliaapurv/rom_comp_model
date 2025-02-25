#include "Config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

static std::string trim(const std::string& s) {
    const std::string whitespace = " \t\r\n";
    size_t start = s.find_first_not_of(whitespace);
    size_t end = s.find_last_not_of(whitespace);
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

Config Config::fromTXT(const std::string& filename)
{
    Config cfg;
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filename);
    }

    std::string line;
    // For each parameter, read one line, strip comments, and parse the first token.
    
    // Read Nx
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading Nx from config file");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.Nx))
            throw std::runtime_error("Error reading Nx");
    }

    // Read Ny
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading Ny");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.Ny))
            throw std::runtime_error("Error reading Ny");
    }

    // Read Lx
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading Lx");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.Lx))
            throw std::runtime_error("Error reading Lx");
    }

    // Read Ly
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading Ly");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.Ly))
            throw std::runtime_error("Error reading Ly");
    }

    // Read dt
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading dt");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.dt))
            throw std::runtime_error("Error reading dt");
    }

    // Read finalTime
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading finalTime");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.finalTime))
            throw std::runtime_error("Error reading finalTime");
    }

    // Read viscosity
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading viscosity");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.viscosity))
            throw std::runtime_error("Error reading viscosity");
    }

    // Read snapshotInterval
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading snapshotInterval");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.snapshotInterval))
            throw std::runtime_error("Error reading snapshotInterval");
    }

    // Read snapshotFile (string)
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading snapshotFile");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        cfg.snapshotFile = trim(line);
        if (cfg.snapshotFile.empty())
            throw std::runtime_error("Error reading snapshotFile");
    }

    // Read numPodModes
    if (!std::getline(ifs, line))
        throw std::runtime_error("Error reading numPodModes");
    {
        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);
        std::istringstream iss(trim(line));
        if (!(iss >> cfg.numPodModes))
            throw std::runtime_error("Error reading numPodModes");
    }

    return cfg;
}
