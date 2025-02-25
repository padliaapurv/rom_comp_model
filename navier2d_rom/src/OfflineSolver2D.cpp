#include "OfflineSolver2D.h"
#include <fstream>
#include <cmath>
#include <iostream>

OfflineSolver2D::OfflineSolver2D(const Config& cfg)
    : cfg_(cfg)
{
    Nx_ = cfg_.Nx;
    Ny_ = cfg_.Ny;
    Lx_ = cfg_.Lx;
    Ly_ = cfg_.Ly;
    dx_ = Lx_ / (Nx_ - 1);
    dy_ = Ly_ / (Ny_ - 1);

    dt_ = cfg_.dt;
    finalTime_ = cfg_.finalTime;
    nu_ = cfg_.viscosity;
    snapshotInterval_ = cfg_.snapshotInterval;
    snapshotFile_ = cfg_.snapshotFile;

    u_.resize(Nx_*Ny_, 0.0);
    v_.resize(Nx_*Ny_, 0.0);
    uNext_.resize(Nx_*Ny_, 0.0);
    vNext_.resize(Nx_*Ny_, 0.0);
}

void OfflineSolver2D::initialize() {
    // e.g. a shear flow or random init
    // let's do something like: u(x,0)=1 at top boundary, rest=0
    // or a "swirl" in the domain
    for(int j=0; j<Ny_; j++){
        for(int i=0; i<Nx_; i++){
            int id=idx(i,j);
            double x = i*dx_;
            double y = j*dy_;
            // For example, a circular swirl
            double r2 = (x-0.5*Lx_)*(x-0.5*Lx_) + (y-0.5*Ly_)*(y-0.5*Ly_);
            if(r2 < 0.05) {
                u_[id] = 1.0;
                v_[id] = 1.0;
            } else {
                u_[id] = 0.0;
                v_[id] = 0.0;
            }
        }
    }
}

// Single explicit time step
// PDE: du/dt = - (u du/dx + v du/dy) + nu (d^2u/dx^2 + d^2u/dy^2)
void OfflineSolver2D::stepExplicit() {
    // We'll do central differences for spatial deriv
    auto lap = [&](const std::vector<double>& phi, int i, int j){
        double left   = phi[idx(i-1,j)];
        double right  = phi[idx(i+1,j)];
        double up     = phi[idx(i,j+1)];
        double down   = phi[idx(i,j-1)];
        double center = phi[idx(i,j)];
        double d2x = (left - 2*center + right)/(dx_*dx_);
        double d2y = (down - 2*center + up   )/(dy_*dy_);
        return d2x + d2y;
    };

    auto dudx = [&](int i, int j){
        double left  = u_[idx(i-1,j)];
        double right = u_[idx(i+1,j)];
        return (right - left)/(2*dx_);
    };
    auto dudy = [&](int i, int j){
        double down = u_[idx(i,j-1)];
        double up   = u_[idx(i,j+1)];
        return (up - down)/(2*dy_);
    };
    auto dvdx = [&](int i, int j){
        double left  = v_[idx(i-1,j)];
        double right = v_[idx(i+1,j)];
        return (right - left)/(2*dx_);
    };
    auto dvdy = [&](int i, int j){
        double down = v_[idx(i,j-1)];
        double up   = v_[idx(i,j+1)];
        return (up - down)/(2*dy_);
    };

    // We'll ignore boundary conditions for now or set them to zero
    for(int j=1; j<Ny_-1; j++){
        for(int i=1; i<Nx_-1; i++){
            int id=idx(i,j);
            double convU = u_[id]*dudx(i,j) + v_[id]*dudy(i,j);
            double convV = u_[id]*dvdx(i,j) + v_[id]*dvdy(i,j);

            double lapU = lap(u_,i,j);
            double lapV = lap(v_,i,j);

            // PDE step
            uNext_[id] = u_[id] + dt_*( -convU + nu_*lapU );
            vNext_[id] = v_[id] + dt_*( -convV + nu_*lapV );
        }
    }

    // simple boundary: set to 0
    for(int i=0; i<Nx_; i++){
        uNext_[idx(i,0)] = 0.0; 
        vNext_[idx(i,0)] = 0.0;
        uNext_[idx(i,Ny_-1)] = 0.0;
        vNext_[idx(i,Ny_-1)] = 0.0;
    }
    for(int j=0; j<Ny_; j++){
        uNext_[idx(0,j)] = 0.0; 
        vNext_[idx(0,j)] = 0.0;
        uNext_[idx(Nx_-1,j)] = 0.0;
        vNext_[idx(Nx_-1,j)] = 0.0;
    }

    // swap
    std::swap(u_, uNext_);
    std::swap(v_, vNext_);
}

void OfflineSolver2D::storeSnapshot(double time) {
    // Flatten [u, v] into an Eigen::VectorXd of length 2*Nx_*Ny_
    int n = 2*Nx_*Ny_;
    Eigen::VectorXd snap(n);
    for(int j=0; j<Ny_; j++){
        for(int i=0; i<Nx_; i++){
            int id = idx(i,j);
            snap(id) = u_[id];
            snap(id + Nx_*Ny_) = v_[id];
        }
    }
    snapshots_.push_back(snap);
}

void OfflineSolver2D::writeSnapshotsToFile() {
    if(snapshots_.empty()) {
        std::cerr << "[OfflineSolver2D] No snapshots!\n";
        return;
    }
    int n = snapshots_[0].size();
    int m = snapshots_.size();
    std::ofstream ofs(snapshotFile_);
    if(!ofs.is_open()){
        std::cerr << "Cannot open " << snapshotFile_ << "\n";
        return;
    }
    ofs << n << " " << m << "\n";
    for(int row=0; row<n; row++){
        for(int col=0; col<m; col++){
            ofs << snapshots_[col](row) << " ";
        }
        ofs << "\n";
    }
    ofs.close();
    std::cout << "[OfflineSolver2D] Wrote " << m 
              << " snapshots to " << snapshotFile_ << std::endl;
}

void OfflineSolver2D::runOfflineSolve() {
    initialize();
    storeSnapshot(0.0);

    int steps = static_cast<int>(std::ceil(finalTime_/dt_));
    for(int s=1; s<=steps; s++){
        stepExplicit();
        if(s % snapshotInterval_ == 0){
            storeSnapshot(s*dt_);
        }
    }
    // store final
    storeSnapshot(finalTime_);

    writeSnapshotsToFile();
}
