/**
 * simple.cu
 * date:		2008.06
 * description:	 	fill an array with consecutive values	
 * notes:		compile with nvcc and parent code, simple.c:
 * 				"nvcc simple.c simple.cu"
 * This program is very similar to a program that appears on a CUDA tutorial
 * published in Dr. Dobbs Journal.
 * The tutorial is available at:
 * http://www.ddj.com/hpc-high-performance-computing/207200659
 * Andrew Bellenir's matrix multiplication was also used as a basis for this example
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cutil.h>
/* 
 * In CUDA it is necessary to define block sizes
 * The grid of data that will be worked on is divided into blocks
 */
#define BLOCK_SIZE 32
/**
 * This is the function that will be executed in each and every one
 * of the stream processors
 * The __global__ directive identifies this function as being
 * an executable kernel on the CUDA device.
 * All kernesl must be declared with a return type void 
 */ 
__global__ void cu_fillArray(int *array_d){
        int x;
	/* blockIdx.x is a built-in variable in CUDA
           that returns the blockId in the x axis
           of the block that is executing this block of code
           threadIdx.x is another built-in variable in CUDA
           that returns the threadId in the x axis
           of the thread that is being executed by this
           stream processor in this particular block
        */
	x=blockIdx.x*BLOCK_SIZE+threadIdx.x;
	array_d[x]=x;
}

/**
 * This function is called in the host computer.
 * It, in turn, calls the function that is executed on the GPU.
 * Recall that:
 *  The host computer and the GPU card have separate memories
 *  Hence it will be necessary to 
 *    - Allocate memory in the memory on the GPU 
 *    - Copy the variables that will be operated from the memory 
 *      in the host to the corresponding variable in the GPU memory
 *    - Describe the configuration of the grid and the block size
 *    - Call the kernel, the code that will be executed on the GPU
 *    - Once the kernel has finished executing, copy back
 *      the results from the memory of the GPU to the memory on the host
 */
extern "C" void fillArray(int *array,int arraySize){
	//a_d is the GPU counterpart of the array that exists on the host memory 
	int *array_d;
	cudaError_t result;
	//allocate memory on device
	// cudaMalloc allocates space in the memory of the GPU card
	result = cudaMalloc((void**)&array_d,sizeof(int)*arraySize);
	if (result != cudaSuccess) {
		printf("cudaMalloc failed.");
		exit(1);
	}
	//copy the array into the variable array_d in the device 
	// The memory from the host is being copied to the corresponding variable
	// in the GPU global memory
	result = cudaMemcpy(array_d,array ,sizeof(int)*arraySize,cudaMemcpyHostToDevice);
	if (result != cudaSuccess) {
		printf("cudaMemcpy failed.");
		exit(1);
	}

	//execution configuration...
	// Indicate the dimension of the block
	dim3 dimblock(BLOCK_SIZE);
	// Indicate the dimension of the grid measured in blocks
	dim3 dimgrid(arraySize/BLOCK_SIZE);
	//actual computation: Call the kernel, the function that is
	// executed by each and every stream processor on the GPU card
	cu_fillArray<<<dimgrid,dimblock>>>(array_d);
	//read results back:
	// Copy the results from the memory in the GPU back to the memory on the host
	result = cudaMemcpy(array,array_d,sizeof(int)*arraySize,cudaMemcpyDeviceToHost);
	if (result != cudaSuccess) {
		printf("cudaMemcpy failed.");
		exit(1);
	}
	// Release the memory on the GPU card
	result = cudaFree(array_d);
	if (result != cudaSuccess) {
		printf("cudaFree failed.");
		exit(1);
	}
}

