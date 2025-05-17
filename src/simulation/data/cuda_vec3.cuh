#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

namespace simulation {
class cudaVec3
{
    // determines if an vector instance 
    // is copied from another one (with copy constructor)
    bool isCopy = false;
public:
	float* x = 0;
	float* y = 0;
	float* z = 0;

	// allocated on host
	cudaVec3(int n);
	cudaVec3(const cudaVec3& other);
	~cudaVec3();

	__device__  inline float3 get(int index) const
    {
        return make_float3(x[index], y[index], z[index]);
    }
    __device__  inline void set(int index, float3 v)
    {
        x[index] = v.x;
        y[index] = v.y;
        z[index] = v.z;
    }
    __device__  inline void add(int index, float3 v)
    {
        x[index] += v.x;
        y[index] += v.y;
        z[index] += v.z;
    }
    __device__  inline void addf(int index, float v)
    {
        x[index] += v;
        y[index] += v;
        z[index] += v;
    }
    // __device__  inline void atomicAddVec3(int index, float3 v)
    // {
    //     atomicAdd(&x[index], v.x);
    //     atomicAdd(&y[index], v.z);
    //     atomicAdd(&z[index], v.y);
    // }
};
}
