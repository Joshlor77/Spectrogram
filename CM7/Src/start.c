#define CPACR				(*(volatile unsigned int *) 0xE000ED88)
#define	NVIC_ISER0			(*(volatile unsigned int *) 0xE000E100)

#define tim2_it				(1 << 28)

#define __CPU_M7

void SystemInit(void){
	//Set full access privilege to enable FPU
	CPACR |= (0xF << 20);
}
