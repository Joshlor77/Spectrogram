#ifndef M7_MPU_H_
#define M7_MPU_H_

#include <stdint.h>

#define MPU_BASE			(0xE000ED90)
#define MPU_TYPE			(*(volatile uint32_t *) (MPU_BASE + 0x4*0))
#define MPU_CTRL			(*(volatile uint32_t *) (MPU_BASE + 0x4*1))
#define MPU_RNR				(*(volatile uint32_t *) (MPU_BASE + 0x4*2))
#define MPU_RBAR			(*(volatile uint32_t *) (MPU_BASE + 0x4*3))
#define MPU_RASR			(*(volatile uint32_t *) (MPU_BASE + 0x4*4))

typedef struct {
	uint32_t IREGIONs;
	uint32_t DREGIONs;
	bool SEPARATE;
} MPUSupport;
MPUSupport mpu_support_info();

typedef struct {
	bool PRIVDEFENA;		//Enables privileged software access to default memory map
	bool HFNIENA;			//Enables operation of MPU during hardfault, NMI, and FAULTMASK handlers
	bool ENABLE;		//Enables MPU
} MPUSettings_CTRL;
void mpu_set_controls(MPUSettings_CTRL s);
void mpu_enable();
void mpu_disable();

typedef enum {
	size32Bytes=4,
	size1KByte=9,
	size1MByte=19,
	size1GByte=29,
	size4GByte=31
} mpu_region_size;

typedef enum {
	StrongOrder=0,
	DeviceShare=1,
	Normal_WT=2,
	Normal_WB=3,
	Normal_NonCacheable=4,
	Normal_WB_WARA=7,
	DeviceNoShare=8,
} mpu_memory_type;

typedef enum {
	pNA_uNA=0, 	//Priv NoAcc     | Unpriv NoAcc
	pRW_uNA=1, 	//Priv ReadWrite | Unpriv NoAcc
	pRW_uRO=2, 	//Priv ReadWrite | Unpriv ReadOnly
	pRW_uRW=3, 	//Priv ReadWrite | Unpriv No ReadWrite
	pRO_uNA=5, 	//Priv ReadOnly  | Unpriv NoAcc
	pRO_uRO=6  	//Priv ReadOnly  | Unpriv ReadOnly
} mpu_AP;

struct MPURegionSettings {
	mpu_AP ap;
	uint8_t isShareable: 1;
	uint8_t isEnabled: 1;
	uint8_t neverExecute: 1;
	uint8_t regionNumber: 4;
	uint8_t subregion_field;
	uint32_t region_base_addr;
	mpu_memory_type mt;
	mpu_region_size size;
};

void mpu_configure_region(MPURegionSettings s);

#endif
