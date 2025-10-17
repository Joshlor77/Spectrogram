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
    	uint32_t _N = N;
    	uint32_t numberOfDFT = 1;
    	uint32_t numBF = 1 << (M-1);
    	uint32_t BFDist = numBF;
    	for (uint32_t m = 0; m < M; m++){
    		for (uint32_t l = 0; l < numberOfDFT; l++){
    			for (uint32_t BF = 0; BF < numBF; BF++){
    				uint32_t p = BF + l*_N;
    				uint32_t q = p + BFDist;
    				complex prevQRes;
    				prevQRes.real = x[q].real * WN[l].real - x[q].imag * WN[l].imag;
    				prevQRes.imag = x[q].real * WN[l].imag + x[q].imag * WN[l].real;
    				x[q].real = x[p].real - prevQRes.real;
    				x[q].imag = x[p].imag - prevQRes.imag;
    				x[p].real = x[p].real + prevQRes.real;
    				x[p].imag = x[p].imag + prevQRes.imag;
    			}
    		}
    		numberOfDFT = numberOfDFT * 2;
    		numBF = numBF / 2;
    		BFDist = numBF;
    		_N = _N / 2;
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
    	for (uint32_t n = 0; n < N; n++){
    		res[n] = sqrtf(powf(x[n].real, 2) + powf(x[n].imag, 2));
    	}
    }
};
