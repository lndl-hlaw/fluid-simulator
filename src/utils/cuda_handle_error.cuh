#pragma once
#include "cuda_runtime.h"
#include <cstdio>
#include <cstdlib>


#ifndef custom_min
#define custom_min(a, b) (a > b ? b : a)
#endif
#ifndef custom_max
#define custom_max(a, b) (a > b ? a : b)
#endif
#ifndef custom_clamp
#define custom_clamp(v, _minimum, _maximum)                                    \
  custom_min(_maximum, custom_max(v, _minimum))
#endif

#ifndef HANDLE_ERROR 
#define HANDLE_ERROR(cmd)                                                         \
  do {                                                                         \
    cudaError_t err = cmd;                                                     \
    if (err != cudaSuccess) {                                                  \
      printf("Failed: Cuda error %s:%d '%s'\n", __FILE__, __LINE__,            \
             cudaGetErrorString(err));                                         \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)
#endif
