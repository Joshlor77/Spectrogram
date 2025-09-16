#pragma once

#include <stdint.h>

#define NUMBER_OF_HSEM		32

class binary_semaphore {
private:
	class hsem_manager {
	protected:
		static bool hsemaphores [NUMBER_OF_HSEM];
		uint16_t procId;
		uint8_t coreID;
		uint8_t hsem;
	};

	hsem_manager man;
public:
	binary_semaphore();

	bool try_acquire();
	void release();
};
