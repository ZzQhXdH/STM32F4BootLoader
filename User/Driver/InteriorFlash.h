#ifndef		__INTERIOR_FLASH_H
#define		__INTERIOR_FLASH_H

#include "stm32f4xx.h"

#define FLASH_KEY1		0x45670123
#define FLASH_KEY2		0xCDEF89AB


class InteriorFlash
{
	public:
		enum FlashSector {
			Sector0 = 0x08000000, // BootLoader 16k
			Sector1 = 0x08004000, // Application 16k
			Sector2 = 0x08008000, // Application 16k
			Sector3 = 0x0800C000, // Application 16k
			Sector4 = 0x08010000, // Application 64k
			Sector5 = 0x08020000, // Application 128k
			Sector6 = 0x08040000, // Application 128k
			Sector7 = 0x08060000, // Application 128k
			Sector8 = 0x08080000, // Application 128k
			Sector9 = 0x080A0000, // Application 128k
			Sector10 = 0x080C0000, // Application 128k
			Sector11 = 0x080E0000, // Application 128k
		};
	public:
		static inline InteriorFlash &instance(void) {
			static InteriorFlash f;
			return f;
		}
		void vEarseApplicationSector(uint32_t size);
		void vWrite4Byte(uint32_t address, uint32_t value);
		void vWriteBuffer(uint32_t address, const uint32_t *buffer, uint32_t size);
		void vReadBuffer(uint32_t address, uint32_t *buffer, uint32_t size) {
			for (uint32_t i = 0; i < size; i ++) {
				buffer[i] = *((volatile uint32_t *) (address + i * 4));
			}
		}
		void vSectorEarse(uint8_t sectorId);

	private:
		InteriorFlash(void);
};



#endif
