#include "simulator.cuh"
#include <cuda_runtime_api.h>
#include <vector>
#include <algorithm>
#include "../global/constants.hpp"
#include "../utils/cu_threads_spatial_division.hpp"
#include "../utils/cuda_handle_error.cuh"
#include "../utils/math.cuh"
#include "data/cuda_vec3.cuh"

namespace simulation {
namespace {

void __global__ applyGravitation(cudaVec3 velocities) {
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id >= PARTICLES_COUNT)
        return;
    velocities.y[id] -= GRAVITY*TIME_STEP; 
}

void __global__ propagateVelocities(cudaVec3 positions, cudaVec3 velocities) {
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id >= PARTICLES_COUNT)
        return;
    float3 prevPosition = positions.get(id);
    float3 velocity = velocities.get(id);
    float3 dP = TIME_STEP*velocity;
    float3 newPosition = prevPosition + dP;

    // Handle collision with ground (y = 0)
    // Simple reflection with damping factor
    if (newPosition.y <= EPSILON) {
        newPosition.y = EPSILON;
        velocities.set(id, -1*VELOCITY_COLLISION_DAMPING*velocity);
    } 
    positions.set(id, newPosition);
}
}
Simulator::Simulator(graphics::points_mesh& particleMesh): 
    interop(particleMesh.getVBO()), particlesPositions(PARTICLES_COUNT), particlesVelocities(PARTICLES_COUNT) 
{
    std::vector<float> vx, vy, vz;
    vx.reserve(PARTICLES_COUNT), vy.reserve(PARTICLES_COUNT), vz.reserve(PARTICLES_COUNT);
    auto vertices = particleMesh.getVertices();
    std::for_each(vertices.begin(), vertices.end(), [&](const graphics::Vec3& v) {
        vx.push_back(v.x);
        vy.push_back(v.y);
        vz.push_back(v.z);
    });

	HANDLE_ERROR(cudaMemcpy(particlesPositions.x, vx.data(), PARTICLES_COUNT * sizeof(float), cudaMemcpyHostToDevice));
	HANDLE_ERROR(cudaMemcpy(particlesPositions.y, vy.data(), PARTICLES_COUNT * sizeof(float), cudaMemcpyHostToDevice));
	HANDLE_ERROR(cudaMemcpy(particlesPositions.z, vz.data(), PARTICLES_COUNT * sizeof(float), cudaMemcpyHostToDevice));
}

void Simulator::simulationStep() {
    utils::cuThreads particlesTh(PARTICLES_COUNT);
    applyGravitation<<<particlesTh.blocks, particlesTh.threadsPerBlock>>>(particlesVelocities);
    propagateVelocities<<<particlesTh.blocks, particlesTh.threadsPerBlock>>>(particlesPositions, particlesVelocities);
    HANDLE_ERROR(cudaDeviceSynchronize());
}
void Simulator::visualizePositions()
{
    interop.mapPointsOnParticles(particlesPositions);
}
}

