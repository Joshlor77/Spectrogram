#include <stdint.h>

#include <gpio.h>
#include <rcc.h>

#include <st7789v3.h>

void init();

const uint32_t chunkSize = 65535;

const uint32_t buffSize = 240 * 320 * 2;
uint8_t buff1[buffSize] = {0};

int main(void)
{
	init();

	st7789v3 display;
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

	display.setColumnAddr(0, 239);
	display.setRowAddr(0, 319);


	uint32_t chunks = (buffSize + chunkSize - 1) / chunkSize;
	uint32_t lastChunkSize = buffSize % chunkSize;
	uint32_t k = 0;
	while(true){
		display.sendCommand(st7789v3::commands::RAMWR);
		for (uint32_t i = 0; i < buffSize; i++){
			buff1[i] = i*k;
			i++;
			buff1[i] = (i-1)*k;
		}
		k++;
		for (uint32_t i = 0; i < (chunks - 1); i++){
			display.sendData(&buff1[i*chunkSize], chunkSize);
		}
		display.sendData(&buff1[chunkSize*(chunks-1)], lastChunkSize);
	}
}

/* C10 SCK
 * C11 MISO (Swapped to MOSI)
 * C12 RES
 * B12 DC	(Data/Command select)
 * A15 NSS
 * C7  BLK	(Black light)
 */

void init(){
	reallocPeripheral(AHB4_Peripheral::hsem);
}
