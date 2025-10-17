#pragma once

#include <stdint.h>
#include <math.h>

#define MATH_PI 3.14159265358979323846264338327950288419716939937510

uint32_t log2floor(uint32_t x){
	uint32_t res = 0;
	while (!( (x>>res)&1) ){
		res++;
	}
	return res;
}

uint32_t bitRev(uint32_t x, uint32_t bits){
	uint32_t res = 0;
	__asm(
		"RBIT %[result], %[input_x]"
		: [result] "=r" (res)
		: [input_x] "r" (x)
	);
	return res >> (32-bits);
}

struct complex {
	float real;
	float imag;
};

void swap(complex& x0, complex& x1){
	float tempReal = x0.real;
	float tempImag = x0.imag;
	x0.real = x1.real;
	x0.imag = x1.imag;
	x1.real = tempReal;
	x1.imag = tempImag;
}

//Only has valid behavior for N as a power of 2 for now.
template <uint32_t N>
class FFTCalculator {
private:
    complex WN[N/2];	//Twiddle Factors
    uint32_t M;
    void calcTwiddles(){
    	float baseParameter = (2 * M_PI) / N;
    	for (uint32_t r = 0; r < N/2; r++){
    		float parameter = baseParameter * r;
    		uint32_t index = bitRev(r,M-1);
    		WN[index].real =  cos(parameter);
    		WN[index].imag = -sin(parameter);
    	}
    }
public:
    FFTCalculator(){
    	M = log2floor(N);
    	calcTwiddles();
    }

    void fft(complex x[]){
    	uint32_t _N = N;		//Current size of each DFT at the current stage
    	uint32_t numberOfDFT = 1;
    	uint32_t numBF = 1 << (M-1);

    	for (uint32_t m = 0; m < M; m++){
    		uint32_t t = 0;
    		for (uint32_t l = 0; l < numberOfDFT; l++){
    			for (uint32_t BF = 0; BF < numBF; BF++){
    				uint32_t p = BF + t;
    				uint32_t q = p + numBF;
    				complex prevQRes;
    				prevQRes.real = x[q].real * WN[l].real;
    				prevQRes.imag = x[q].real * WN[l].imag;
    				prevQRes.real -= x[q].imag * WN[l].imag;
    				prevQRes.imag += + x[q].imag * WN[l].real;
    				x[q].real = x[p].real - prevQRes.real;
    				x[q].imag = x[p].imag - prevQRes.imag;
    				x[p].real = x[p].real + prevQRes.real;
    				x[p].imag = x[p].imag + prevQRes.imag;
    			}
    			t += _N;
    		}
    		numberOfDFT = numberOfDFT * 2;
    		numBF = numBF / 2;
    		_N = _N / 2;
    	}
    }

    void rfft(complex x[], complex y[]){
        //Pack y into the imaginary parts of x
        for (uint32_t i = 0; i < N; i++){
            x[i].imag = y[i].real;
        }
        fft(x);
        for (uint32_t n = 0; n < N; n++){
            y[n].real = x[n].imag + x[(N-n) % N].imag;
            y[n].imag = x[n].real - x[(N-n) % N].real;
        }
        for (uint32_t n = 0; n < N/2 + 1; n++){
            complex currentxn = x[n];
            complex mirrorxn = x[(N-n) % N];

            x[n].real = currentxn.real + mirrorxn.real;
            x[n].imag = currentxn.imag - mirrorxn.imag;

            x[(N-n) % N].real = mirrorxn.real + currentxn.real;
            x[(N-n) % N].imag = mirrorxn.imag - currentxn.imag;
        }
        for (uint32_t n = 0; n < N; n++){
            y[n].real *= 0.5;
            y[n].imag *= 0.5;
            x[n].real *= 0.5;
            x[n].imag *= 0.5;
        }
    }

    void reversePermute(complex x[]){
    	uint32_t revN;
    	for (uint32_t n = 0; n < N/2; n++){
    		revN = bitRev(n, M);
    		if (n < revN){
    			swap(x[n], x[revN]);
    			swap(x[(~n) & (N-1)], x[(~revN) & (N-1)]);
    		}
    		n++;
    		swap(x[n], x[bitRev(n, M)]);
    	}
    }

    void magnitude(complex x[], float res[]){
    	uint32_t i = 0;
    	for (uint32_t n = 0; n < N; n++){
    		i++;
    		res[n] = sqrtf(x[n].real * x[n].real + x[n].imag * x[n].imag);
    	}
    }
};
