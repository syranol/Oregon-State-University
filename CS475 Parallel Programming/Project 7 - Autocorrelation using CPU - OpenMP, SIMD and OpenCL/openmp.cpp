#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#define NUMT         1
#define ARRAYSIZE    4000 // you decide
#define NUMTRIES     100   // you decide

#define SIZE 32768
float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

float Array[2*SIZE];
float Sums[1*SIZE];

int main( )
{
    #ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
    #endif

    FILE *  fp;
    int     i;
    int size;
    double maxMegaMults = 0.;

    fp = fopen( "signal.txt", "r" );
    if( fp == NULL )
    {
        fprintf( stderr, "Cannot open file 'signal.txt'\n" );
        exit( 1 );
    }
    fscanf( fp, "%d", &Size );
    size = SIZE;

    for( i = 0; i < Size; i++ )
    {
        fscanf( fp, "%f", &Array[i] );
        Array[i+Size] = Array[i];       // duplicate the array
    }
    fclose( fp );

    omp_set_num_threads( NUMT );


    #pragma omp parallel for
    for( int shift = 0; shift < Size; shift++ )
    {
        double time0 = omp_get_wtime( );
        float sum = 0.;
        for( int i = 0; i < Size; i++ )
        {
            sum += Array[i] * Array[i + shift];
        }
        Sums[shift] = sum;  // note the "fix #2" from false sharing if you are using OpenMP

        double megaMults = (double)size/(time1-time0)/1000000.;
        if( megaMults > maxMegaMults )
        maxMegaMults = megaMults;
        double time1 = omp_get_wtime( );
    }

    printf( "Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults );

        return 0;
       }
//
