#include "gl_interop.cuh"
#include "cuda_gl_interop.h"
#include "../../utils/cuda_handle_error.cuh"
#include "../../global/constants.hpp"
#include "../../utils/cu_threads_spatial_division.hpp"

namespace simulation {
namespace {
// get CUDA a pointer to openGL buffer
void* mapResourceAndGetPointer(cudaGraphicsResource_t resource)
{
    void* resourceBuffer = 0;
    size_t numBytes;
    HANDLE_ERROR(cudaGraphicsMapResources(1, &resource, 0));
    HANDLE_ERROR(cudaGraphicsResourceGetMappedPointer((void**)&resourceBuffer, &numBytes, resource));
    return resourceBuffer;
}


__global__ void glInteropMap3Floats(float* devCudaOffsetBuffer, cudaVec3 positions)
{
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id >= PARTICLES_COUNT)
        return;

    // Insert any debug position changes here

    devCudaOffsetBuffer[3 *id] = positions.x[id];
    devCudaOffsetBuffer[3 * id + 1] = positions.y[id];
    devCudaOffsetBuffer[3 * id + 2] = positions.z[id];

}
}

glInterop::glInterop(unsigned int meshVBO) {
    HANDLE_ERROR(cudaGraphicsGLRegisterBuffer(&particleResource, meshVBO, cudaGraphicsRegisterFlagsNone));
}


void glInterop::mapPointsOnParticles(cudaVec3 positions) {

		// get CUDA a pointer to openGL buffer
		float* devCudaOffsetBuffer = (float*)mapResourceAndGetPointer(particleResource);
		
		// translate our CUDA positions into Vertex offsets
        utils::cuThreads mappingThreads(PARTICLES_COUNT);
		glInteropMap3Floats << <mappingThreads.blocks, mappingThreads.threadsPerBlock >> > (devCudaOffsetBuffer, positions);

		HANDLE_ERROR(cudaGraphicsUnmapResources(1, &particleResource, 0));
}
}
