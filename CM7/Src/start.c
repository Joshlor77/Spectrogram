#define CPACR				(*(volatile unsigned int *) 0xE000ED88)

void SystemInit(void){
	//Set full access privilege to enable FPU
	CPACR |= (0xF << 20);
}
