#include <stdint.h>
#include <cmath>

#include <fft.h>

#define CPACR				(*(volatile unsigned int *) 0xE000ED88)

int main(void)
{
	//Enable FPU
	CPACR |= (0xF << 20);

	complex x[32] = {0};

	x[0].real = 1;
	x[1].real = 1;
	x[2].real = 1;
	x[3].real = 1;
	x[4].real = 1;

	FFTCalculator<32> fft32;

	fft32.fft(x);
	fft32.reversePermute(x);
	float x_mag[32];
	fft32.magnitude(x, x_mag);

	while(true){

	}
}
