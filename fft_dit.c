#include "fft.h"

int fix_fft(fixed fr[], fixed fi[], int m, int inverse)
{
    int mr,nn,i,j,l,k,istep, n, scale, shift;
    register FFT_REG_SIMD simd_fr, simd_fi;
    
    fixed qr,qi;		//even input
    fixed tr,ti;		//odd input
    fixed wr,wi;		//twiddle factor
    
    //number of input data
    n = 1<<m;

    if(n > N_WAVE) return -1;

    mr = 0;
    nn = n - 1;
    scale = 0;

    /* decimation in time - re-order data */
    for(m=1; m<=nn; ++m) {
        l = n;
        do{
        	l >>= 1;
        }while(mr+l > nn);
        mr = (mr & (l-1)) + l;

        if(mr <= m) continue;
        tr = fr[m];
        fr[m] = fr[mr];
        fr[mr] = tr;
        
        ti = fi[m];
        fi[m] = fi[mr];
        fi[mr] = ti;
    }
	
    
    l = 1;
    k = LOG2_N_WAVE-1;
    while(l < n)
    {
        if(inverse)
        {
            /* variable scaling, depending upon data */
            shift = 0;
            for (i = 0; i < (n / 8); i += 8){
            if (FFT_SHIFT_CHECK(fr, i) | FFT_SHIFT_CHECK(fi, i)){
            	shift = 1;
            	++scale;
            	break;
            	}
            }
        	}
         else
         {
           shift = 1;

         }

        istep = l << 1;

        printf("l = %d\n", l);
        switch(l)
        {
        case 1:
        for (i = 0; i < n; i += 8)
        {
        simd_fr = FFT_SIMD_LOAD(fr, i);
        simd_fi = FFT_SIMD_LOAD(fi, i);
        FFT_SIMD_STAGE_01(simd_fr, simd_fi, shift);
        FFT_SIMD_STORE(fr, i, simd_fr);
        FFT_SIMD_STORE(fi, i, simd_fi);
        }
        break;
        case 2:
        for (i = 0; i < n; i += 8)
        {
        simd_fr = FFT_SIMD_LOAD(fr, i);
        simd_fi = FFT_SIMD_LOAD(fi, i);
        FFT_SIMD_STAGE_02(simd_fr, simd_fi, shift, inverse);
        FFT_SIMD_STORE(fr, i, simd_fr);
        FFT_SIMD_STORE(fi, i, simd_fi);
        }
        break;
        case 4:
        WUR_FFT_SIMD_K(7);
        for (i = 0; i < n; i += 8)
        {
        simd_fr = FFT_SIMD_LOAD(fr, i);
        simd_fi = FFT_SIMD_LOAD(fi, i);
        FFT_SIMD_STAGE_03(i, simd_fr, simd_fi, shift, inverse);
        FFT_SIMD_STORE(fr, i, simd_fr);
        FFT_SIMD_STORE(fi, i, simd_fi);
        }
        break;
        }
        --k;
        l = istep;
    }

    return scale;
}







