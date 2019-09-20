/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/*
 To compile: 
    nvcc -arch=sm_60 reduction_kernel.cu
 To run with the array size 2^20, expo dist mean 5, and init seed 17:
    ./a.out 20 5 17
 */
#include <stdio.h>
#include <stdlib.h>

template<class T>
struct SharedMemory
{
    __device__ inline operator       T *()
    {
        extern __shared__ int __smem[];
        return (T *)__smem;
    }

    __device__ inline operator const T *() const
    {
        extern __shared__ int __smem[];
        return (T *)__smem;
    }
};

template<class T>
T reduceCPU(T *data, int size)
{
    T sum = data[0];
    T c = (T)0.0;

    for (int i = 1; i < size; i++)
    {
        T y = data[i] - c;
        T t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}

/*
    Parallel sum reduction using shared memory
    - takes log(n) steps for n input elements
    - uses n threads
    - only works for power-of-2 arrays
*/

/*
    This version uses sequential addressing -- no divergence or bank conflicts.
*/
__global__ void
qroot(double *g_idata, double *g_odata)
{
	 unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
	 g_odata[i]= sqrt(sqrt(g_idata[i]));
} 
__global__ void
reduce(double *g_idata, double *g_odata, unsigned int n)
{
    double *sdata = SharedMemory<double>();

    // load shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;

    sdata[tid] = (i < n) ? g_idata[i] : 0;

    __syncthreads();

    // do reduction in shared mem
    for (unsigned int s=blockDim.x/2; s>0; s>>=1)
    {
        if (tid < s)
        {
            sdata[tid] += sdata[tid + s];
        }

        __syncthreads();
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}


// CUDA Runtime
#include <cuda_runtime.h>

#define checkCudaErrors(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

extern "C" double reduction(int n, int mean, int seed)
{
        int size = 1<<n;    // number of elements to reduce
        int maxThreads = 256;  // number of threads per block

        // create random input data on CPU
        unsigned int bytes = size * sizeof(double);

        double *h_idata = (double *) malloc(bytes);

        srand48(seed);
        for (int i=0; i<size; i++)
        {
                // h_idata[i] = 1.0; // for testing
                // expo dist with mean 5.0
                h_idata[i] = -mean * log(drand48());
        }

        int numBlocks = size / maxThreads;
        int numThreads = size;

        int smemSize = maxThreads * sizeof(double);

        // allocate mem for the result on host side
        double *h_odata = (double *) malloc(numBlocks*sizeof(double));

        // allocate device memory and data
        double  *d_idata = NULL;
        double *d_odata = NULL;

        checkCudaErrors(cudaMalloc((void **) &d_idata, bytes));
        checkCudaErrors(cudaMalloc((void **) &d_odata, numBlocks*sizeof(double)));

        // copy data directly to device memory
        checkCudaErrors(cudaMemcpy(d_idata, h_idata, bytes, cudaMemcpyHostToDevice));
		qroot<<<numBlocks,maxThreads,smemSize>>>(d_idata, d_idata);
        reduce<<<numBlocks,maxThreads,smemSize>>>(d_idata, d_odata, numThreads);

        int s=numBlocks;

        while (s > 1) {
            reduce<<<(s+maxThreads-1)/maxThreads,maxThreads,smemSize>>>(d_odata, d_odata, s);
            s = (s+maxThreads-1)/maxThreads;
        }

        checkCudaErrors(cudaMemcpy(h_odata, d_odata, sizeof(double), cudaMemcpyDeviceToHost));

        printf("GPU sum : %f\n\n", h_odata[0]);
		double result = h_odata[0];

        checkCudaErrors(cudaFree(d_idata));
        checkCudaErrors(cudaFree(d_odata));

        double cpu_result = reduceCPU<double>(h_idata, size);
   
        printf("CPU sum : %f\n", cpu_result);
        
    return result;
}
