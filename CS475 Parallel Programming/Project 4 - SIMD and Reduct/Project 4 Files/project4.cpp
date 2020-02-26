//Use the supplied SIMD SSE code to run array miltiplication timing experience

//Runt he same experience for a second time using my own C++ array multiplication code

//Use the supplied SIMD SSE code to run array mutiplication + reduction timing experiment

//Run the same experiment using your own C++ array multiplication + reudction code

//Use different array sizes from 1K to "big"

//Run each array-size test a certain amount of trials, use peak for performance record

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "simd.p4.h"


//this is a random generator taken form pervious assignment

unsigned int seed = 0;

float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

int main()
{
	 #ifndef _OPENMP
        fprintf( stderr, "OpenMP is not available\n" );
        return 1;
    #endif

    srand(time(NULL));
    // Use array sizes form 1k to big
    int arrayLength[10] = {1000,2000,4000,8000,16000,32000,64000,128000,256000,512000};

    for(int i = 0; i < 10; i++)
    {

    	// Setting up data

    	//declaration of arrays to be filled with data
    	float *arrayOne = new float[arrayLength[i]];

    	float *arrayTwo = new float[arrayLength[i]];

    	// used for computing results
    	float *result = new float[arrayLength[i]];




    	//filling array with random data
    	for(int j = 0; j < arrayLength[i]; j++)
    	{
    		arrayOne[j] = Ranf(&seed,0,10);

    		arrayTwo[j] = Ranf(&seed,0,10);
    	}

    	//omp start
    	double start = omp_get_wtime();
    	for(int k = 0; k < arrayLength[i]; k++)
    	{
    		result[k] = arrayOne[k] * arrayTwo[k];
    	}
    	//omp end
    	double end = omp_get_wtime();
   
   		//print result
    	printf("\nREGMUL Size %d\t wTime %lf ",arrayLength[i], end-start);

    	//set data
    	for(int j = 0; j < arrayLength[i]; j++)
    	{
    		arrayOne[j] = Ranf(&seed,0,10);

    		arrayTwo[j] = Ranf(&seed,0,10);
    	}

    	//omp start
    	start = omp_get_wtime();
    	SimdMul(arrayOne, arrayTwo, result, arrayLength[i]);
    	end = omp_get_wtime();
    	//omp end

    	//print result
    	printf("\nSIMDMUl Size %d\t wTime %lf ",arrayLength[i], end-start);

    	//set data
	    for(int j = 0; j < arrayLength[i]; j++)
	    {
	    	arrayOne[j] = Ranf(&seed,0,10);

    		arrayTwo[j] = Ranf(&seed,0,10);
	    }

	    float sum = 0;
	    //omp start
	    start = omp_get_wtime();
	    

	    for(int j = 0; j < arrayLength[i]; j++)
	    {
	    	sum = sum + (arrayOne[i] * arrayTwo[i]);
	    }
	    end = omp_get_wtime();
	    //omp end

	    //print result
	   	printf("\nREGSUM Size %d\t wTime %lf ",arrayLength[i], end-start);

	   	//set data
    	for(int j = 0; j < arrayLength[i]; j++)
    	{
    		arrayOne[j] = Ranf(&seed,0,10);

    		arrayTwo[j] = Ranf(&seed,0,10);
    	}
    	//omp start
    	start = omp_get_wtime();
    	SimdMulSum(arrayOne,arrayTwo,arrayLength[i]);
    	end = omp_get_wtime();
    	//omp end

    	//print result
    	printf("\nSIMDSUM Size %d\t wTime %lf ",arrayLength[i], end-start);
	}
	return 0;
}
