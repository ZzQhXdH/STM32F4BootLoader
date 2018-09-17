#include "stm32f4xx.h"
#include "Driver/FlashMemory.h"
#include "Driver/InteriorFlash.h"
#include "Driver/LogUart.h"
#include "Custom/OTAManager.h"
#include "Driver/MainUart.h"

void vTimer10Init(void);


int main(void)
{
	MainUart::instance();
	OTAManager::instance().update();
	
	uint32_t sp = *((volatile uint32_t *) 0x8004000);
	uint32_t addr = *((volatile uint32_t *) (0x8004000 + 4));
	void (*fun)(void) = (void (*)(void)) addr;
	__set_MSP(sp);
	fun();
}

void vTimer10Init(void)
{
	RCC->APB2ENR |= 0x20000; // 17
	
	TIM10->CR1 = 0x00;
	TIM10->CR2 = 0x00;
	TIM10->PSC = 168 - 1;
	TIM10->ARR = 0xFFFF;
	TIM10->CR1 = 0x01;
	TIM10->EGR = 0x01;
}
	
void vDelayUs(uint16_t us)
{
	TIM10->CNT = 0x00;
	while (TIM10->CNT <= us);
}




