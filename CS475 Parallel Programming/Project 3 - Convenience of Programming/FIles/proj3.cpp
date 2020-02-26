
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define NUMT 4
//Global Variables
int NowYear = 2019;		// 2019 - 2024
int	NowMonth = 0;		// 0 - 11

float NowPrecip = 6.0;		// inches of rain per month
float NowTemp = 50.0;		// temperature this month
float NowHeight = 1;		// grain height in inches
int	NowNumDeer = 1;		// number of deer in the current population
int NowNumGrassHopper = 1;

//Interestign paraemters
const float GRAIN_GROWS_PER_MONTH = 8.0;
const float ONE_DEER_EATS_PER_MONTH = 0.5;

const float AVG_PRECIP_PER_MONTH = 6.0;	// average
const float AMP_PRECIP_PER_MONTH = 6.0;	// plus or minus
const float RANDOM_PRECIP = 2.0;	// plus or minus noise

const float AVG_TEMP = 50.0;	// average
const float AMP_TEMP = 20.0;	// plus or minus
const float RANDOM_TEMP = 10.0;	// plus or minus noise

const float MIDTEMP = 40.0;
const float MIDPRECIP = 10.0;


float SQR( float x )
{
        return x*x;
}

float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}


//GrainDeer - COmputer the next grain height and the next number of deers
//based on the current set of global state variables. They will compute these
//into local, temporary variables. They will both hit Done COmputing barrier
void GrainDeer()
{
	//If # of graindeer > number of inches of the grain
	//Decrease graindeer by one

	//else increase graindeer by one

	while(NowYear < 2025)
	{
		int tempDeer = NowNumDeer;
		
		if((float)tempDeer > NowHeight)
		{
			tempDeer--;
		}
		else
		{
			tempDeer++;
		}

		//DoneComputing
		#pragma omp barrier

		NowNumDeer = tempDeer;
		//DoneAssigning
		#pragma omp barrier

		//DonePrinting
		#pragma omp barrier
	}
}

//Grain - COmputer the next grain height and the next number of deers
//based on the current set of global state variables. They will compute these
//into local, temporary variables. They will both hit Done COmputing barrier
void Grain()
{
	
	while(NowYear < 2025)
	{
		//Check if condition is good by and using sqr fn
		float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
		float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

		float tempHeight = NowHeight;
		//clamp NowHeight against zero?

		tempHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
	 	tempHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

	 	if(tempHeight < 0.)
	 	{
	 		tempHeight = 0.;
	 	}

	 	//DoneComputing
	 	#pragma omp barrier

	 	NowHeight = tempHeight;

	 	//DoneAssigning
	 	#pragma omp barrier

	 	//DonePrinitng
	 	#pragma omp barrier
	}
}

//Watcher - Once GrainDeer and Grain are done, print current set of global state
//variables, icnrement month count and then use the new month to compute the new
//temperature and percipitation.
void Watcher()
{

	while(NowYear < 2025)
	{
		//DoneComputing
		#pragma omp barrier

		//DoneAssigning
		#pragma omp barrier


		printf("\nNowYear: %d\t NowMonth: %d\tNumHeight: %f\tNowNumDeer: %d\t NowNumGrassHopper: %d\t",NowYear, NowMonth, NowHeight, NowNumDeer, NowNumGrassHopper);
		
		NowMonth++;

		if(NowMonth == 12)
		{
			NowMonth = 0;
			NowYear++;
		}

		unsigned int seed = 0;  // a thread-private variable

		//temperature and precipitation 
		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
		float temp = AVG_TEMP - AMP_TEMP * cos( ang );

		NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
		
		if( NowPrecip < 0. )
			NowPrecip = 0.;

		printf("Temp: %f\t Precip: %f\n", NowTemp, NowPrecip);
		//Done prinitng
		#pragma omp barrier
	}
}
//MyAgent - My own factors that will affect grain height and number of the deers
void MyAgent()
{

	while(NowYear < 2025)
	{
		int tempGrassHopper = NowNumGrassHopper;
		float tempGHeight = NowHeight;	
		//GrassHoppers
		if(NowHeight > 5)
		{
			if(tempGrassHopper == 0)
			{
				tempGrassHopper =  1;
			}
			else
			{
			tempGrassHopper = tempGrassHopper + 5;
			}
			
			tempGHeight = NowHeight - 3 ;
		}
		else
		{
			tempGrassHopper = tempGrassHopper - 1;
		}

		if(tempGrassHopper < 0)
		{
			tempGrassHopper = 0;
		}


		//DoneComputing
		#pragma omp barrier

		NowNumGrassHopper = tempGrassHopper;
		NowHeight = tempGHeight;
	
		//DoneAssigning
		#pragma omp barrier

		//DonePrinting
		#pragma omp barrier
	}
}
int main()
{	
	#ifndef _OPENMP
		fprintf(stderr, "OpenMP is not available\n" );
		return 1;
	#endif
	srand(time(NULL));

	//three barriers each 
	omp_set_num_threads(NUMT);	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer( );
		}

		#pragma omp section
		{
			Grain( );
		}

		#pragma omp section
		{
			Watcher( );
		}

		#pragma omp section
		{
			MyAgent( );	// your own
		}
	}
	return 0;

		// implied barrier -- all functions must return in order
		// to allow any of them to get past here
}
