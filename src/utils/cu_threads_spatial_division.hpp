#pragma once
#include "cuda_runtime.h"
#include <iostream>
#include <math.h>

namespace utils {
class cuThreads {
protected:
    static constexpr unsigned int maxCudaThreadsPerBlock = 576; // 1024;
public:
    dim3 threadsPerBlock;
    dim3 blocks;
    cuThreads() {}

    cuThreads(unsigned int overallThreadCount)
        : threadsPerBlock(std::min(overallThreadCount, maxCudaThreadsPerBlock)),
            blocks(std::ceil(static_cast<float>(overallThreadCount) / threadsPerBlock.x)) {}
};

struct cuThreads2D : public cuThreads {
    static constexpr unsigned int equal2DThreadsPerBlock = 24; // 32;
public:
    cuThreads2D(unsigned int threadsX, unsigned int threadsY) {
        unsigned int overallThreadCount = threadsX * threadsY;
        if (overallThreadCount < maxCudaThreadsPerBlock) {
            threadsPerBlock = dim3(overallThreadCount);
            blocks = dim3(1);
        } else {
            threadsPerBlock = dim3(equal2DThreadsPerBlock, equal2DThreadsPerBlock);
            blocks = dim3(
            std::ceil(static_cast<float>(threadsX) / equal2DThreadsPerBlock),
            std::ceil(static_cast<float>(threadsY) / equal2DThreadsPerBlock)
            );
        }
    }
};
} // namespace utils
