#include <stdint.h>
#include <st7789v3.h>
#include <nvic.h>

const uint32_t chunkSize = 65535;
const uint32_t buffSize = 240 * 320 * 2;
uint8_t buff1[buffSize] = {0};

st7789v3 display;

void init_st7789v3();


int main(void)
{
	init_st7789v3();

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
