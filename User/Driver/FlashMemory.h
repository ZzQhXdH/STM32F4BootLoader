#ifndef		__FLASH_MEMORY_H
#define		__FLASH_MEMORY_H

#include "stm32f4xx.h"

#define ROBOT_ARM1_CHECK_DATA_INDEX			0
#define ROBOT_ARM2_CHECK_DATA_INDEX			1

/**
 * @brief 软件上FLASH存储器将抽象成一个32位数的二维数组
 * FlashArray[0][1024] 其中索引表示扇区位置
 */
class FlashMemory 
{
	public:
		static inline FlashMemory &instance(void) {
			static FlashMemory m;
			return m;
		}
		
		void vReadData(uint32_t address, uint8_t *data, uint32_t size);
		void vWriteData(uint32_t address, const uint8_t *data, uint32_t size);
		bool xWriteAndCheck(uint32_t address, const void *data, uint32_t size);
		void vSectorEarse(uint32_t address);
		uint16_t xGetId(void);
		
	private:
		FlashMemory(void);
		void initSpiInterface(void);
	
		uint8_t xTranslateByte(uint8_t byte);
		uint8_t xQueryStatus(void);
		void vWriteEnable(void);
		void vPageProgram(uint32_t address, const uint8_t *data, uint32_t size);
	
		inline void vCsLow(void) { GPIOB->BSRRH = 0x40;}
		inline void vCsHigh(void) { GPIOB->BSRRL = 0x40;}
		
	private:
		uint32_t mBuffer[1024];
};





#endif
