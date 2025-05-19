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

    if (newPosition.x <= XMIN) {
        newPosition.x = XMIN + EPSILON;
        velocity.x *= -1*VELOCITY_COLLISION_DAMPING;
    }

    if (newPosition.x >= XMAX) {
        newPosition.x = XMAX - EPSILON;
        velocity.x *= -1*VELOCITY_COLLISION_DAMPING;
    }

    if (newPosition.y <= YMIN) {
        newPosition.y = YMIN + EPSILON;
        velocity.y *= -1*VELOCITY_COLLISION_DAMPING;
    }

    if (newPosition.y >= YMAX) {
        newPosition.y = YMAX - EPSILON;
        velocity.y *= -1*VELOCITY_COLLISION_DAMPING;
    }

    if (newPosition.z <= ZMIN) {
        newPosition.z = ZMIN + EPSILON;
        velocity.z *= -1*VELOCITY_COLLISION_DAMPING;
    }

    if (newPosition.z >= ZMAX) {
        newPosition.z = ZMAX - EPSILON;
        velocity.z *= -1*VELOCITY_COLLISION_DAMPING;
    }

    positions.set(id, newPosition);
    velocities.set(id, velocity);
}

// Kernel K(r) = || (r_0 - r)^2 || 
__device__ float smoothingKernel(float distance, float radius) {
    if (distance >= radius) {
        return .0f;
    }
    float volume = PI*pow(radius, 4.0f)/6.0f; 
    return (radius - distance)*(radius - distance)/volume;
}

__device__ float smoothingKernelDerivative(float distance, float radius) {
    if (distance >= radius) {
        return .0f;
    }
    float scalingFactor = 12.0f/(PI*pow(radius, 4.0f));
    return (distance - radius)*distance*scalingFactor;
}

// __device__ float smoothingKernel(float distance, float radius) {
//     if (distance >= radius) {
//         return 0.0f;
//     }
//     
//     // Using standard cubic spline kernel for better stability
//     float q = distance / radius;
//     float factor = 8.0f / (PI * pow(radius, 3.0f));
//     
//     if (q <= 0.5f) {
//         return factor * (6.0f * (pow(q, 3.0f) - pow(q, 2.0f)) + 1.0f);
//     } else {
//         return factor * 2.0f * pow(1.0f - q, 3.0f);
//     }
// }
//
// __device__ float smoothingKernelDerivative(float distance, float radius) {
//     if (distance >= radius || distance < EPSILON) {
//         return .0f;
//     }
//     
//     // Gradient of cubic spline kernel
//     float q = distance / radius;
//     float factor = 48.0f / (PI * pow(radius, 4.0f));
//     float gradient_magnitude;
//     
//     if (q <= 0.5f) {
//         gradient_magnitude = factor * (3.0f * q * q - 2.0f * q);
//     } else {
//         gradient_magnitude = factor * -pow(1.0f - q, 2.0f);
//     }
//     
//     // Return gradient vector (points away from center)
//     return (gradient_magnitude / distance);
// }
void __global__ calculateDensities(cudaVec3 positions, float* densities) {
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id >= PARTICLES_COUNT)
        return;

    float3 myPos = positions.get(id);
    float density = 0.0f;
    for (int i = 0; i < PARTICLES_COUNT ; ++i) {
        if (i == id) {
            density += smoothingKernel(.0f, NEIGH_RADIUS);
        } else {
            float distanceSq = length_squared(positions.get(i) - myPos);
            if (distanceSq < NEIGH_RADIUS_SQUARED) {
                float distance = sqrtf(distanceSq);
                density += smoothingKernel(distance, NEIGH_RADIUS); 
            }
        }
    }
    densities[id] = density;
}

void __global__ calculatePressureForces(cudaVec3 positions, cudaVec3 velocities, float* densities) {
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id >= PARTICLES_COUNT)
        return;

    float pressure = 0.0f;
    float3 myPos = positions.get(id);
    float3 pressureForce = make_float3(.0f, .0f, .0f);
    float densityA = densities[id];
    for(int i = 0; i < PARTICLES_COUNT; ++i) {
        if (i == id) {
            continue;
        }
        float3 r = positions.get(i) - myPos;
        float distanceSq = length_squared(r);
        if (distanceSq > EPSILON && distanceSq < NEIGH_RADIUS_SQUARED) {
            float distance = sqrtf(distanceSq);
            float3 dir = r/distance;
            float densityB = densities[i];
            float pressureA = STIFFNESS_PRESSURE*max(.0f, densityA - REST_DENSITY);
            float pressureB = STIFFNESS_PRESSURE*max(.0f, densityB - REST_DENSITY);
            pressureForce += (-(pressureA + pressureB)*smoothingKernelDerivative(distance, NEIGH_RADIUS)/(2.0f*densityB))*dir;
        }
    }
    velocities.add(id, TIME_STEP*pressureForce/densityA);
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
    HANDLE_ERROR(cudaMalloc((void**)&densities, PARTICLES_COUNT*sizeof(float)));
}

void Simulator::simulationStep() {
    utils::cuThreads particlesTh(PARTICLES_COUNT);

    // assume gravitation
    applyGravitation<<<particlesTh.blocks, particlesTh.threadsPerBlock>>>(particlesVelocities);

    // calculate overall pressures on every particle
    calculateDensities<<<particlesTh.blocks, particlesTh.threadsPerBlock>>>(particlesPositions, densities);
    calculatePressureForces<<<particlesTh.blocks, particlesTh.threadsPerBlock>>>(particlesPositions, particlesVelocities, densities);
    
    // propagate computed velocities into positions
    // This kernel also occasionally handle collisions
    propagateVelocities<<<particlesTh.blocks, particlesTh.threadsPerBlock>>>(particlesPositions, particlesVelocities);

    HANDLE_ERROR(cudaDeviceSynchronize());
}
void Simulator::visualizePositions()
{
    interop.mapPointsOnParticles(particlesPositions);
}
}

