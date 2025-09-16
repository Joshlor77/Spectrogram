#include "mpu.h"

//Region should only be between 0-7 or 0-15 depending on supported regions
void mpu_select_region(uint8_t reg){
	MPU_RNR &= ~(0xFF);
	MPU_RNR |= reg;
}

/* Set the region size before setting the base address.
 * Sets a new base address for the selected region
 * The number of bits the addr needs to be depends on the size.
 * You can get this number N from the mpu_region_size enum values + 1.
 */
void mpu_set_region_baseAddr(uint32_t addr){
	uint32_t N = ((MPU_RASR & 0x1F) >> 1) + 1;
	MPU_RBAR |= (addr<<N);
}

//Set the size of the selected region. Set region size before setting the region base addr.
void mpu_set_region_size(mpu_region_size size){
	MPU_RASR &= ~(0x1Fu<<1); //clear SIZE
	MPU_RASR |= (size<<1);
}

//Set the memory type of the selected region
void mpu_set_region_mem_type(mpu_memory_type mt){
	MPU_RASR &= ~(0x1Fu<<16); //Clear TEX, C, B
	MPU_RASR |= mt;
}

//Only affects normal memory types
void mpu_set_region_shareable(bool shareable){
	MPU_RASR &= ~(1u<<18);
	MPU_RASR |= (shareable<<18);
}

void mpu_enable_region(){
	MPU_RASR |= 1;
}

void mpu_disable_region(){
	MPU_RASR &= ~1u;
}
void mpu_neverExecRegion(){
	MPU_RASR |= (1<<28);
}
void mpu_allowExecRegion(){
	MPU_RASR &= ~(1<<28);
}
void mpu_set_subregion_field(uint8_t field){
	MPU_RASR |= (field<<8);
}
void mpu_set_AP(mpu_AP ap){
	MPU_RASR &= ~(0x3<<24);
	MPU_RASR |= (ap<<24);
}

void mpu_configure_region(MPURegionSettings s){
	mpu_select_region(s.regionNumber);
	mpu_set_region_size(s.size);
	mpu_set_region_baseAddr(s.region_base_addr);
	mpu_set_region_shareable(s.isShareable);
	mpu_set_subregion_field(s.subregion_field);
	mpu_set_AP(s.ap);
	(s.neverExecute) ? mpu_neverExecRegion() : mpu_allowExecRegion();
	(s.isEnabled) ? mpu_enable_region() : mpu_disable_region();
}


void mpu_set_controls(MPUSettings_CTRL s){
	MPU_CTRL &= ~(0x7u);
	if (s.PRIVDEFENA)
		MPU_CTRL |= (1<<2);
	if (s.HFNIENA)
		MPU_CTRL |= (1<<1);
	if (s.ENABLE)
		MPU_CTRL |= (1<<0);
	return;
}

MPUSupport mpu_support_info(){
	MPUSupport s;
	s.IREGIONs = MPU_TYPE & (0xFF<<16);
	s.DREGIONs = MPU_TYPE & (0xFF<<8);
	s.SEPARATE = MPU_TYPE & 1;
	return s;
}

void mpu_enable(){
	MPU_CTRL |= 1;
}
void mpu_disable(){
	MPU_CTRL &= ~(1u);
}
