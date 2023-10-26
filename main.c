//main.c


#include 		"fft.h"
#include        <stdio.h>
#include        <math.h>

#define DO_FFT	1

#define M       3

//number of points
#define N       (1<<M)

fixed real[N], imag[N];


int main()
{
    int i;

    for(i=0; i<N; i++)
    {
        real[i] = 1000*cos(i*2*3.1415926535/N);
        imag[i] = 0;
    }
    
    printf("\nInput Data\n");
    for (i=0; i<N; i++)
    {
    	printf("%d: %d, %d\n", i, real[i], imag[i]);
    }
	
    
    //FFT
    //#if (DO_FFT==1)
    fix_fft(real, imag, M, 0);
    
    printf("\nFFT\n");
    for (i=0; i<N; i++)
    {
        printf("%d: %d, %d\n", i, real[i], imag[i]);
    }
	
    //#else
    //IFFT
    fix_fft(real, imag, M, 1);
    


    printf("\nIFFT\n");
    for (i=0; i<N; i++)
    {
        printf("%d: %d, %d\n", i, real[i], imag[i]);
    }
    
//#endif
    
    return 0;
}

