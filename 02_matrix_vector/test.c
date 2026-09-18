/*
 * Copyright (C) 2022-2024 University of Bologna
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 * Authors: Manuele Rusci 	  UniBO (manuele.rusci@unibo.it)
 * 			Lorenzo Lamberti  UniBO (lorenzo.lamberti@unibo.it)
 */
#include "pmsis.h"
#include "stdbool.h" // to use "bool" data type

/*  defines */
#define N 50 // the matrix size is NxM, and the vector size is M.
#define M 50
#define MAT_EL (2) // matrix constant values
#define VEC_EL (4) // vector constant values
int mac_counter=0;

/* Allocation of IO variables into L2 memory */
// input variables
PI_L2 int matrix[N * M]; // the matrix as an array of size N*M
PI_L2 int vector[M];	 // the vector as an array of size M
// output variable
PI_L2 int output_vec[N]; // N*M x M*1 -> N*1
/*  Note: PI_L2 is an attribute for "forcing" allocation in L2 memory.
	Ref: /pulp/pulp-sdk/rtos/pulpos/common/include/pos/data/data.h:54: */

void start_perf_counter()
{
	// enable the perf counter of interest
	pi_perf_conf(	1 << PI_PERF_/*YOUR_CODE_HERE*/ | 	// count cycles
					1 << PI_PERF_/*YOUR_CODE_HERE*/ );	// count instructions
	// reset the perf counters
	pi_perf_reset();
	//  start the perf counter
	pi_perf_start();
}

void stop_perf_counter()
{
	// stop the perf counter
	pi_perf_stop();
	// collect and print statistics
	uint32_t cycles_counter = pi_perf_read(PI_PERF_CYCLES);
	uint32_t instr_counter = pi_perf_read(PI_PERF_INSTR);

	/*
		TASK 2.2
		Measure:
			- How many MAC operations are needed for the gemv
		Calculate:
			- CPI
			- MACs/Cycles
			- Instructions/Cycles
			- Instructions/ MACs
	*/

	// N° Multiply Accumulate Operations (MACs)
		/* already done with task 2.1. use the mac_counter global variable */
	// CPI = cycles / number_of_instructions_executed
	float cpi = /* YOUR CODE HERE*/
	// MAC/Cycles
	float mac_on_cycles = /* YOUR CODE HERE*/
	// Instructions/Cycles
	float instructions_on_cycles = /* YOUR CODE HERE*/
	// Instructions/MAC
	float instructions_on_mac = /* YOUR CODE HERE*/

	// print results
	printf("--- Performances ---------\n");
	printf("Cycles: %d \n", cycles_counter); 					// this comes from the performance counter
	printf("N° of Intructions: %d\n", instr_counter); 			// this comes from the performance counter
	printf("mac: %d \n", mac_counter); 							// you must measure this
	printf("CPI: %f \n", cpi); 									// calculate it
	printf("Instructions/Cycles: %f \n", instructions_on_cycles); 	// calculate it
	printf("Instructions/MAC: %f \n", instructions_on_mac); 		// calculate it
	printf("-------------------------\n");

}

// print array elements
void print_array(int * A_ar, int size)
{
	for(int i=0;i<size;i++)
		printf("%d ", A_ar[i]); // print array element
	printf("\n");

}

/* generic matrix-vector multiplication */
int __attribute__((noinline)) gemv(int size_N, int size_M, int *mat_i, int *vec_i, int *vec_o)
{
	/*
	TASK 2.1
	- you must count the MAC by increasing a counter in the inner loop of the gemv.
	- use the "mac_counter" global variable
	*/
	mac_counter=0;
	for (int i = 0; i < size_N; i++) // outer looop of the gemv
	{
		for (int j = 0; j < size_M; j++) // inner loop of the gemv
		{
			// multiply accumulate operation (MAC)
			vec_o[i] += mat_i[i * size_M + j] * vec_i[j];
			//*(vec_o+i) += *(mat_i+i*M+j)*(*(vec_i+j)); // try to uncomment this and comment the above line. You will notice a speedup in cycles
		}
	}
}

int main()
{
	// Initialization of operands: matrix
	for (int i = 0; i < (N * M); i++)
	{
		matrix[i] = MAT_EL;
	}
	// Initialization of operands: vector
	for (int i = 0; i < M; i++)
	{
		vector[i] = VEC_EL;
	}
	// Initialization of the output to 0
	for (int i = 0; i < N; i++)
	{
		output_vec[i] = 0;
	}
	printf("\n");

	/* call the GEneric Matrix-Vector (gemv) function */
	gemv(N, M, matrix, vector, output_vec);

	// print and check the results
	printf("\nThe %d output elements are: \n", N);
	print_array(output_vec, N);

	// check here the results
	int correctness = 1;
	for (int i = 0; i < N; i++)
	{
		if (output_vec[i] != (M * MAT_EL * VEC_EL))
		{
			correctness = 0;
			break;
		}
	}
	printf(correctness ? "\nRESULTS MATCHING: correct\n" : "RESULTS NOT MATCHING: not correct\n");
}
