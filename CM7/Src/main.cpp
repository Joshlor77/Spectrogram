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

const uint32_t N = 2048;
__attribute__((section(".dtcm_ram"))) FFTCalculator<N> fft1024;
__attribute__((section(".dtcm_ram"))) complex x[N];
__attribute__((section(".dtcm_ram"))) complex y[N];

uint32_t addr = (uint32_t) &x;

const uint32_t adcBuffLen = 32;
uint32_t adcBuff[adcBuffLen];

uint32_t averagetime = 0;
uint32_t totalTime = 0;

uint32_t averagetimeNew = 0;
uint32_t totalTimeNew = 0;

void benchmarkMyFFT(){
	uint32_t iterations = 500;
	fft1024.fft(x);

	float res[N];

	for (uint32_t i = 0; i < iterations; i++){
		uint16_t start = TIM3->CNT;

		fft1024.magnitude(x, res);

		uint16_t end = TIM3->CNT;
		uint16_t diff = end - start;
		totalTime += diff;
		averagetime = totalTime / (i+1);
	}
}

int main(void)
{
//	init_st7789v3();
//	init_ADC();
//	adc_start(ADC1);
	reallocPeripheral(APB1L_Peripheral::tim3);
	tim_prescale(TIM3, 64); //Timer 3 set to 1MHz
	tim_enable(TIM3);

	for(uint32_t i = 0; i < N; i++){
		x[i].real = i / 10.0;
		x[i].imag = 0.0;
		y[i].real = i * 0.01;
		y[i].imag = 0.0;
	}

	benchmarkMyFFT();

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
