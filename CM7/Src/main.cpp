#include <stdint.h>
#include <st7789v3.h>
#include <nvic.h>
#include <rcc.h>
#include <adc.h>
#include <tim2345.h>
#include <gpio.h>
#include <dma.h>
#include <dmamux.h>

#include <fft.h>

void init_st7789v3();
void init_ADC();

void swapBuffer();
void sendCurrentBuffer();

st7789v3 display;

	uint32_t adcBuff[2*100];
//	reallocPeripheral(APB1L_Peripheral::tim3);
//	tim_prescale(TIM3, 64); //Timer 3 set to 1MHz
//	tim_enable(TIM3);
//	init_ADC();
//	adc_start(ADC1);

//Returns the reverse bit of a number for a certain number of bits
uint16_t _REV16(uint16_t x){
	uint16_t res;
	__asm(
		"REV16 %[result], %[input_x]"
		: [result] "=r" (res)
		: [input_x] "r" (x)
	);
	return res;
}


union rgb16 {
	//This is ordered bgr so that the bytes are sent correctly through SPI. Make sure to reverse the byte order to send correctly.
	struct rgb565 {
		uint16_t b : 5;
		uint16_t g : 6;
		uint16_t r : 5;
	} rgb;
	uint16_t data;
};

int main(void)
{
	const int N = 16;
	const int L = 8;
	const int O = 4;

    complex x[N] = {
        1,0,
        2,0,
        1,0,
        2,0,
        3,0,
        2,0,
        1,0,
        2,0,
        1,0,
        0,0,
        1,0,
        2,0,
        3,0,
        1,0,
        2,0,
        3,0
    };

    float w[L];
    int H = L - O;
    int frames = 1 + (N-L) / (L-O);

    complex v[L] = {0};
    hanning(L,w);
    float X[frames][N/2 + 1] = {0};

    for (int f = 0; f < frames; f++){
        for (int m = 0; m < L; m++){
            v[m].real = w[m] * x[m + f*H].real;
        }
        fft<L>(v);
        reversePermute(v, L, log2floor(L));
        magnitude(v, X[f], L);
    }

	init_st7789v3();

	rgb16 color = {0};
	display.setColumnAddr(0, display.MAX_COLS - 1);
	display.setRowAddr(0, display.MAX_ROWS - 1);

	display.sendCommand(st7789v3::commands::RAMWR);
	for (uint32_t i = 0; i < display.MAX_COLS*display.MAX_ROWS; i++){
		display.sendData((uint8_t*) &color.data, 2);
	}

	display.setColumnAddr(display.MAX_COLS/4, 3*display.MAX_COLS/4 - 1);
	display.setRowAddr(display.MAX_ROWS/4, 3*display.MAX_ROWS/4 - 1);
	display.sendCommand(st7789v3::commands::RAMWR);


	while(true){
		color.rgb.g = 0;
		color.rgb.r = 0;
		color.rgb.b = 0;
		for (int i = 0; i < 32; i++){
			for (int i = 0 ; i < 600; i++){
				uint16_t data = _REV16(color.data);
				display.sendData((uint8_t*) &data, 2);
			}
			color.rgb.g =  16*(0.5 - 0.5*cosf((2*MATH_PI*(i))/(32-1)));
			color.rgb.r = 28*(0.5 - 0.5*cosf((2*MATH_PI*(i+32))/(128-1)));
			color.rgb.b = 24*(0.5 - 0.5*cosf((2*MATH_PI*(i+16))/(64-1)));
		}
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
	uint8_t colmod = 0x55;
	display.sendData(&colmod, 1);
	display.sendCommand(st7789v3::commands::MADCTL);
	uint8_t madctl = 0b000000000;
	display.sendData(&madctl, 1);
}

void init_ADC(){
	reallocPeripheral(AHB1_Peripheral::adc1_adc2);
	reallocPeripheral(AHB1_Peripheral::dma1);
	reallocPeripheral(AHB4_Peripheral::gpioa);
	reallocPeripheral(APB1L_Peripheral::tim2);

	//AR value set to overrun one times a second;
	tim_auto_reload(TIM2, 64000000);
	TIMSettingsCR s;
	s.MMS = tim_mms::Update;
	tim_controls(TIM2, s);

	DMAMUX1_ChannelCfg muxC;
	muxC.channel = DMAMUX1_Channel::Ch1;
	muxC.inputRequest = DMAMUX1_MultiplexerInput::adc1_dma;
	muxC.numberForwardDMARequests = 1 - 1;
	dmamux_configChannel(muxC);

	DMA_StreamCfg_PerToMem dmaC;
	dmaC.stream = DMA_Stream::Stream1;
	dmaC.fifoThreshold = DMA_FifoThreshold::Full;
	dmaC.numberOfDataToTransfer = 4;
	dmaC.enableMemIncr = true;
	dmaC.enablePeriphMemIncr = false;
	dmaC.isCircularMode = true;
	dmaC.memory0Addr = (uint32_t) &adcBuff;
	dmaC.periphMemoryAddr = (uint32_t) &(ADC1->DR);
	dmaC.memoryDataSize = DMA_DataSize::Word;
	dmaC.periphDataSize = DMA_DataSize::Word;
	dmaC.memoryBurst = DMA_BurstType::SingleTransfer;
	dmaC.periphBurst = DMA_BurstType::SingleTransfer;
	dma_configStream(dmaC, DMA1);

	adcCom_set_ckmode(ADC12_COMMON, SYNC_DIV2);
	adc_preselectCh(ADC1, 15);
	adc_set_regSeq(ADC1, 1, 15);
	adc_set_chSampleTime(ADC1, 0b111, 15);
	adc_set_boost(ADC1, boost11);
	adc_wake(ADC1);
	adc_calibrate(ADC1, SingleEnded, true);
	ADCSettingsCFGR c;
	c.EXTEN = 1; //Hardware trigger on rising edge
	c.EXTSEL= 11; //Select external trigger event 11 (tim2_trgo)
	c.DMNGT = 0b11;
	adc_set_configurations1(ADC1, c);
	adc_enable(ADC1);

	tim_enable(TIM2);
	dma1_enableStream(DMA_Stream::Stream1);
}
