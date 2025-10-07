#include <stdint.h>
#include <st7789v3.h>
#include <nvic.h>

void init_st7789v3();

const uint32_t chunkSize = 65535;
const uint32_t buffSize = 240 * 320;

const uint32_t chunks = (buffSize + chunkSize - 1) / chunkSize;
const uint32_t extra = buffSize % chunkSize;

uint8_t buff1[buffSize];
uint8_t buff2[buffSize];
uint8_t* currentBuffer = buff1;

st7789v3 display;

void swapBuffer(){
	if (currentBuffer == buff1){
		currentBuffer = buff2;
	} else {
		currentBuffer = buff1;
	}
}

void sendCurrentbuffer(){
	for (uint32_t i = 0; i < chunks - 1; i++){
		display.sendData(&currentBuffer[chunkSize * i], chunkSize);
	}
	display.sendData(&currentBuffer[chunkSize * (chunks-1)], extra);
}

int main(void)
{
	while(true){

	}
}


void init_st7789v3(){
	display.enableBacklight();
	display.sendCommand(st7789v3::commands::SWRESET, false);
	display.sendCommand(st7789v3::commands::SLPOUT);
	display.sendCommand(st7789v3::commands::NORON);
	display.sendCommand(st7789v3::commands::DISPON);
	display.sendCommand(st7789v3::commands::INVON);
	display.sendCommand(st7789v3::commands::COLMOD);
	uint8_t colmod = 0b01010101;
	display.sendData(&colmod, 1);
	display.sendCommand(st7789v3::commands::MADCTL);
	uint8_t madctl = 0b000000000;
	display.sendData(&madctl, 1);
}
