#pragma once
#include "../graphics/mesh/mesh.hpp"
#include "interop/gl_interop.cuh"

namespace simulation {

class Simulator {
private:
    glInterop interop;
    cudaVec3 particlesPositions;
    cudaVec3 particlesVelocities;
public:
    Simulator(graphics::points_mesh& particleMesh);

    void simulationStep();
    void visualizePositions();
};
}
