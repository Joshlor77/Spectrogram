//#include <adc.h>
//#include <dma.h>
//#include <dmamux.h>
//#include <gpio.h>
//#include <rcc.h>
//#include <tim2345.h>
//#include <spi.h>
//
////These are left as examples for myself to look at later to remember how to initialize the peripherals.
//
//void adcInit(){
//	//Reset and allocate ADC1/2
//	adcCom_set_ckmode(ADC12_COMMON, SYNC_DIV2);
//	adc_preselectCh(ADC1, 15);
//	adc_set_regSeq(ADC1, 1, 15);
//	adc_set_chSampleTime(ADC1, 0b111, 15);
//	adc_set_boost(ADC1, boost11);
//	adc_wake(ADC1);
//	adc_calibrate(ADC1, SingleEnded, true);
//	ADCSettingsCFGR c;
//	c.EXTEN = 1; //Hardware trigger on rising edge
//	c.EXTSEL= 11; //Select external trigger event 11 (tim2_trgo)
//	c.DMNGT = 0b11;
//	adc_set_configurations1(ADC1, c);
//	adc_enable(ADC1);
//}
//
//void timInit(){
//	//AR value set to overrun one times a second;
//	tim_auto_reload(TIM2, 64000000);
//	TIMSettingsCR s;
//	s.MMS = tim_mms::Update;
//	tim_controls(TIM2, s);
//	tim_enable(TIM2);
//}
//
//void dmamuxInit(){
//	DMAMUX1_ChannelCfg cfg;
//	cfg.channel = DMAMUX1_Channel::Ch0;
//	cfg.inputRequest = DMAMUX1_MultiplexerInput::adc1_dma;
//	cfg.numberForwardDMARequests = 1 - 1;
//	dmamux_configChannel(cfg);
//}
//
//void dmaInit(){
//	DMA_StreamCfg_PerToMem cfg;
//	cfg.stream = DMA_Stream::Stream0;
//	cfg.fifoThreshold = DMA_FifoThreshold::Full;
//	cfg.numberOfDataToTransfer = 4;
//	cfg.enableMemIncr = true;
//	cfg.enablePeriphMemIncr = false;
//	cfg.isCircularMode = true;
//	cfg.isDoubleBuffer = true;
////	cfg.memory0Addr = (uint32_t) &dArray;
////	cfg.memory1Addr = (uint32_t) &kArray;
//	cfg.periphMemoryAddr = (uint32_t) &(ADC1->DR);
//	cfg.memoryDataSize = DMA_DataSize::Word;
//	cfg.periphDataSize = DMA_DataSize::Word;
//	cfg.memoryBurst = DMA_BurstType::SingleTransfer;
//	cfg.periphBurst = DMA_BurstType::SingleTransfer;
//	dma_configStream(cfg, DMA1);
//}
//
//void peripheralsInit(){
//	reallocPeripheral(APB1L_Peripheral::tim2);
//	reallocPeripheral(AHB1_Peripheral::adc1_adc2);
//	adcInit();
//	timInit();
//	dmamuxInit();
//	dmaInit();
//	dma1_enableStream(DMA_Stream::Stream0);
//}
//
//void GC9A01_TxData(){
//	setPin(PortC, P11);
//}
//void GC9A01_TxCommand(){
//	resetPin(PortC, P11);
//}
//
//enum class GC9A01_Select {
//	Data = 0,
//	Command = 1
//};
//
//void GC9A01_sendDataFrames(uint16_t items, GC9A01_Select select){
//	((bool) select) ? GC9A01_TxCommand() : GC9A01_TxData();
//	dma1_disableStream(DMA_Stream::Stream0);
//	spi_disable(SPI3);
//	SPI3.CFG1 &= ~(1<<15);
//
//	spi_setTransfercount(items, 0, SPI3);
//	dma1_clearAllFlags(DMA_Stream::Stream1);
//	dma1_setDataTransferSize(DMA_Stream::Stream1, items);
//	dma1_enableStream(DMA_Stream::Stream1);
//	SPI3.CFG1 |= (1<<15);
//	spi_enable(SPI3);
//	spi_masterStart(SPI3);
//}
//
//void GC9A01_WaitTxComplete(){
//	while (!dma1_readItrFlag(DMA_Stream::Stream1, DMA_ItrStatus::TransferComplete));
//	while (!(SPI3.SR & (1<<3)));
//	SPI3.IFCR = (1<<4);
//}
//
//
//void initSPI(){
//	reallocPeripheral(AHB1_Peripheral::dma1);
//	reallocPeripheral(APB1L_Peripheral::spi3);
//	reallocPeripheral(AHB4_Peripheral::gpioa);
//	reallocPeripheral(AHB4_Peripheral::gpioc);
//	conf_PinMode(PortA, P15, PinMode::AltFunc);
//	conf_PinMode(PortC, P10, PinMode::AltFunc);
//	conf_PinMode(PortC, P12, PinMode::AltFunc);
//	conf_AltFunc(PortA, P15, 6);		//PA15 set to SPI3_NSS		| CS
//	conf_AltFunc(PortC, P10, 6);		//PC10 set to SPI3_SCK		| SCL
//	conf_AltFunc(PortC, P12, 6);		//PC12 set to SPI3_MOSI		| SDA
//
//	conf_PinMode(PortC, P11, PinMode::Output);		//PC11 to be used for DC with GC9A01
//	conf_PinMode(PortC, P7, PinMode::Output);		//PC7  to be used for RST with GC9A01
//
//	RCC.D2CCIP1R |= (0b100<<12);	//Select Per_CK for SPI123 Ker clock
//	SPI_MasterCFG SPIcfg;
//	SPIcfg.spiMode = SPI_Mode::Mode0;
//	SPIcfg.ssPol = SPI_SSPolarity::Low;
//	SPIcfg.commMode = SPI_CommMode::Simplex;
//	SPIcfg.masterPresc = SPI_MasterBaudRatePresc::Div256;
//	SPIcfg.enableSSOutput = true;
//	SPIcfg.periphControlsGPIOAltFunc = true;
//	SPIcfg.enableTxDMA = true;
//	SPIcfg.interDataIdleness = 0;
//	SPIcfg.SSIdleness = 0;
//	SPIcfg.bitsPerDataFrame = 8 - 1;
//	spi_config(SPIcfg, SPI3);
//
//	DMA_StreamCfg_MemToPer DMAcfg;
//	DMAcfg.stream = DMA_Stream::Stream1;
//	DMAcfg.fifoThreshold = DMA_FifoThreshold::Full;
//	DMAcfg.numberOfDataToTransfer = 4;
//	DMAcfg.enableMemIncr = true;
//	DMAcfg.enablePeriphMemIncr = false;
//	DMAcfg.isCircularMode = false;
//	DMAcfg.isDoubleBuffer = false;
//	DMAcfg.isNotDirectMode = true;
//	DMAcfg.memory0Addr = (uint32_t) &SPIBuffer[0];
//	DMAcfg.periphMemoryAddr = (uint32_t) &(SPI3.TXDR);
//	DMAcfg.memoryDataSize = DMA_DataSize::Byte;
//	DMAcfg.periphDataSize = DMA_DataSize::Byte;
//	DMAcfg.memoryBurst = DMA_BurstType::SingleTransfer;
//	DMAcfg.periphBurst = DMA_BurstType::SingleTransfer;
//	dma_configStream(DMAcfg, DMA1);
//
//	DMAMUX1_ChannelCfg MUXcfg;
//	MUXcfg.channel = DMAMUX1_Channel::Ch1;
//	MUXcfg.inputRequest = DMAMUX1_MultiplexerInput::spi3_tx_dma;
//	MUXcfg.numberForwardDMARequests = 1 - 1;
//	dmamux_configChannel(MUXcfg);
//}
