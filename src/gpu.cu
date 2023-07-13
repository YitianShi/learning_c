#include<iostream>
#include<algorithm>
#include<vector>
#include<cuda_runtime.h>
#include <cstdlib>
#include <cublas_v2.h>
#include "NvInfer.h"
#include "NvInferPlugin.h"
#include "NvInferRuntimeCommon.h"
#include "NvInferRuntime.h"
#include "NvInferVersion.h"
#include "NvOnnxParser.h"
#include "NvInferPluginUtils.h"
#include "NvOnnxConfig.h"


using namespace std;
const int BLOCK= 25;
__global__
void vectAdd(float* A, float* B, float* C, int n)
{
    int i = threadIdx.x + blockDim.x*blockIdx.x;
    if (i<n) C[i] = A[i]+B[i];
}

__global__
void vectMul(float* A, float* B, float* C, int M, int N, int K)
{
    int i = threadIdx.x + blockDim.x*blockIdx.x;
    int j = threadIdx.y + blockDim.y*blockIdx.y;
    if (i < N && j < M)
    {
    float sum=0;
    for (size_t k=0; k<K; k++)
    {
        sum += A[j*K + k] * A[k*N + i];
    }
    C[j*N + i] = sum;
    };
}

__global__
void vectMul2(float* A, float* B, float* C, int width, int numARow, int numBCol)
{
    __shared__ float sharedM[BLOCK][BLOCK];
    __shared__ float sharedN[BLOCK][BLOCK];

    int bx = blockIdx.x;
    int by = blockIdx.y;
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int r = by*BLOCK + ty;
    int c = bx*BLOCK + tx;

    float Csub = 0.0;
    for (int i = 0; i < (int)(ceil((float)width / BLOCK));i++)
    {
        if (i*BLOCK + tx < width && r < numARow)
        sharedM[ty][tx] = A[r*width + i*BLOCK + tx];
        else
        sharedM[ty][tx] = 0.0;

        if (i*BLOCK + ty < width && c < numBCol)
        sharedN[ty][tx] = B[(i*BLOCK + ty)*width + c];
        else
        sharedN[ty][tx] = 0.0;

        __syncthreads();

        for (int j = 0; j< BLOCK; j++)
        Csub += sharedM[ty][j] * sharedN[j][tx];
        __syncthreads();
    }

    if (r<numARow && c<numBCol)
    C[r*numBCol + c] = Csub;
}



int main(int argc, char *argv[])
{    
    int n = atoi(argv[1]);
    int size = n*sizeof(float);
    cout<<n<<endl;

    float *A = (float *)malloc(size);
    float *B = (float *)malloc(size);
    float *C = (float *)malloc(size);

    for (int i = 0; i < n; i++) {
        float af = rand() / double(RAND_MAX);
        float bf = rand() / double(RAND_MAX);
        A[i] = af;
        B[i] = bf;
    }

    float* A_d, *B_d, *C_d, time_0;

    cudaStream_t stream;
    cudaEvent_t event, event2;
    cudaStreamCreate(&stream);
    cudaEventCreate(&event);
    cudaEventCreate(&event2);

    cudaMalloc((void**) &A_d, size);
    cudaMalloc((void**) &B_d, size);
    cudaMemcpyAsync(A_d, A, size, cudaMemcpyHostToDevice, stream);
    cudaMemcpyAsync(B_d, B, size, cudaMemcpyHostToDevice, stream);
    cudaMalloc((void**) &C_d, size);
    cudaMemcpyAsync(C, C_d, size, cudaMemcpyDeviceToHost, stream);

    int thread_per_block = 256;
    int block_per_grid = (n + thread_per_block-1)/thread_per_block;

    dim3 DimGrid(block_per_grid, 1, 1);
    dim3 DimBlock(thread_per_block, 1, 1);

    //vectAdd <<<block_per_grid,thread_per_block, 0, stream>>>(A_d, B_d, C_d, n);
    cudaEventRecord(event, stream);
    vectMul2 <<<block_per_grid,thread_per_block, 0, stream>>>(A_d, B_d, C_d, n, n, n);
    cudaStreamSynchronize(stream);
    cudaEventRecord(event2, stream);

    cudaEventSynchronize(event);
    cudaEventSynchronize(event2);
    cudaEventElapsedTime(&time_0, event, event2);

    cudaStreamDestroy(stream);
    cudaEventDestroy(event);
    cudaEventDestroy(event2);

    cudaFree(A_d);
    cudaFree(B_d);
    cudaFree(C_d);

    printf("%f ms",time_0);
}

void cublas_try(unsigned int m, unsigned int n, unsigned int k,
                cudaStream_t &streamId){

    //Initialize matrices in host
    float *h_A, *h_B, *h_C;

    unsigned int size_A = m*k;
    unsigned int mem_A = sizeof(float)*size_A;
    for (int i = 0; i < size_A; i++) {
        float af = rand() / double(RAND_MAX);
        h_A[i] = af;
    }

    unsigned int size_B = k*n;
    unsigned int mem_B = sizeof(float)*size_B;
    for (int i = 0; i < size_B; i++) {
        float af = rand() / double(RAND_MAX);
        h_B[i] = af;
    }

    unsigned int size_C = m*n;
    unsigned int mem_C = sizeof(float)*size_C;

    //Initialize matrices in device

    float *d_A, *d_B, *d_C;
    cudaMalloc((void**) &d_A, mem_A);
    cudaMalloc((void**) &d_B, mem_B);
    cudaMalloc((void**) &d_C, mem_C);
    cudaMemcpy(d_A, h_A, size_A, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size_B, cudaMemcpyHostToDevice);
    
    dim3 DimGrid(1, 1);
    dim3 DimBlock(1, 1);

    float alpha=1.0f;
    float beta=0.0f;
    int batch = 25;
    long long int stride_A = 1, stride_B = 1, stride_C = 1;

    //create
    cublasHandle_t handle;
    cublasCreate(&handle);

    // cublasStatus_t cublasIsamax(handle, len, *x, row, *res); //cublasIsamin
    
    /*  cublasSgemv(
        handle, op,
        m, n, 
        &alpha,
        *A, &col_A (if op is CUBLAS_OP_T else (CUBLAS_OP_N) &row_A), 
        &x, &x_row, 
        &beta, 
        &y, y_row)
        
        alpha*op(A)*x + beta*y

        cublasSgemm(
        handle, op_a, op_b, 
        m, n, k, 
        &alpha, 
        *A, &col_A (if op is CUBLAS_OP_T else (CUBLAS_OP_N) &row_A)
        *B, &col_B (if op is CUBLAS_OP_T else (CUBLAS_OP_N)&row_B),
        &beta, 
        *C, &row_C)

        C = alpha*op(A)*op(B) + beta*C
    */

    cublasSgemm(
        handle, CUBLAS_OP_N, CUBLAS_OP_N, 
        m, n, k, 
        &alpha, 
        d_B, n, 
        d_A, k,
        &beta, 
        d_C, n);
    // C_T(n*m) = B_T (n*k) * A_T (k*m)

    cublasSgemmBatched(
        handle, CUBLAS_OP_N, CUBLAS_OP_N, 
        m, n, k, 
        &alpha, 
        &d_B, n,
        &d_A, k,
        &beta, 
        &d_C, n, batch);
    //segmented sum
    
    cublasSgemmStridedBatched(
        handle, CUBLAS_OP_N, CUBLAS_OP_N, 
        m, n, k, 
        &alpha, 
        d_B, n, stride_A,
        d_A, k, stride_B,
        &beta, 
        d_C, n, stride_C, 
        batch);
    // C+i*strideC = alpha*op(A+StrideA)*op(B+strIdeB) + beta*(C+i*strideC)

    
    cudaDataType_t A_type = CUDA_R_16F, B_type = CUDA_R_16F, C_type = CUDA_R_16F;
    cublasComputeType_t Com_type = CUBLAS_COMPUTE_16F;
    cublasGemmEx(
        handle, CUBLAS_OP_N, CUBLAS_OP_N, 
        m, n, k, 
        &alpha, 
        d_B, B_type, n,
        d_A, A_type, k,
        &beta, 
        d_C, C_type, n, 
        Com_type, CUBLAS_GEMM_DEFAULT);
    

    //stream
    /*
    cublasSetStream(handle, streamId);
    cublasGetStream(handle, &streamId);
    */

    

    //Calculate
    //cublasSgemm()

    cublasDestroy(handle);
    cudaMemcpy(h_C, d_C, mem_C, cudaMemcpyDeviceToHost);


}




