#include "Driver/InteriorFlash.h"
#include "Driver/LogUart.h"

void vDelayUs(uint16_t us);

#define WaitBusy()		while ( (FLASH->SR & 0x1000) != 0x00)
#define UnLock()		FLASH->KEYR = FLASH_KEY1; FLASH->KEYR = FLASH_KEY2
#define Lock()			FLASH->CR |= 0x80000000

InteriorFlash::InteriorFlash(void)
{
	
}

void InteriorFlash::vEarseApplicationSector(uint32_t size)
{
	vSectorEarse(0x01);
	if (size > 16 * 1024) {
		vSectorEarse(0x02);
	}
	if (size > 16 * 1024 + 16 * 1024) {
		vSectorEarse(0x03);
	}
	if (size > 16 * 1024 + 16 * 1024 + 16 * 1024) {
		vSectorEarse(0x04);
	}
}

static uint8_t GetStatus(void)
{
	uint32_t ret = FLASH->SR;
	if (ret & 0x10000) return 1;
	if (ret & 0x10) return 2;
	if (ret & 0x20) return 2;
	if (ret & 0x40) return 2;
	if (ret & 0x80) return 2;
	return 0;
}

static uint8_t WaitDone(uint32_t timeOut)
{
	uint8_t ret;
	do {
		ret = GetStatus();
		if (ret != 1) break;
		vDelayUs(10);
		timeOut --;
	} while (timeOut > 0);
	return ret;
}

void InteriorFlash::vSectorEarse(uint8_t sectorId)
{
	uint8_t ret = 0;
	UnLock();
	FLASH->ACR &= ~(1 << 10);
	ret = WaitDone(200000);
	if (ret == 0)
	{
		FLASH->CR &= ~(3 << 8); 
		FLASH->CR |= 2 << 8; // 设置为32bit
		FLASH->CR &= ~(0x1F << 3);
		FLASH->CR |= sectorId << 3;
		FLASH->CR |= 1 << 1; // 扇区擦除
		FLASH->CR |= 1 << 16; // 开始擦除
		ret = WaitDone(200000);
		if (ret != 1)
		{
			FLASH->CR &= ~(1 << 1); // 清除擦除标志
		}
	}
	FLASH->ACR |= 1 << 10;
	Lock();
}

void InteriorFlash::vWrite4Byte(uint32_t address, uint32_t value)
{
	uint8_t ret = WaitDone(0xFF);
	if (ret == 0x00)
	{
		FLASH->CR &= ~(3 << 8);
		FLASH->CR |= 2 << 8;
		FLASH->CR |= 1 << 0;
		*((volatile uint32_t *) address) = value;
		ret = WaitDone(0xFF);
		if (ret != 1)
		{
			FLASH->CR &= ~(1 << 0);
		}
	}
}

void InteriorFlash::vWriteBuffer(uint32_t address, const uint32_t *buffer, uint32_t size)
{
	UnLock();
	for (uint32_t i = 0; i < size; i ++)
	{
		vWrite4Byte(address, buffer[i]);
		address += 4;
	}
	Lock();
}









