#pragma once
#include "cuda_runtime.h"
#include "../../graphics/mesh/mesh.hpp"
#include "../data/cuda_vec3.cuh"

namespace simulation {

class glInterop {
    cudaGraphicsResource_t particleResource;
public:
    glInterop(unsigned int meshVBO);
    void mapPointsOnParticles(cudaVec3 positions);
};

}
