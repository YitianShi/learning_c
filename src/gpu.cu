#include<iostream>
#include<algorithm>
#include<vector>
#include<cuda_runtime.h>
#include <cstdlib>
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



