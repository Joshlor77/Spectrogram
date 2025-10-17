#define CPACR				(*(volatile unsigned int *) 0xE000ED88)

#define CCR					(*(volatile unsigned int *) 0xE000ED14)

void SystemInit(void){
	//Set full access privilege to enable FPU
	CPACR |= (0xF << 20);

//	//Enable I-cache
//	CCR |= (1<<17);
}
