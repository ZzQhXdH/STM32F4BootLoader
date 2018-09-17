#ifndef		__MAIN_UART_H
#define		__MAIN_UART_H

#include "stm32f4xx.h"


class MainUart 
{
	public:
		static MainUart &instance(void) {
			static MainUart uart;
			return uart;
		}
		
		void write(uint8_t byte);
		
		void write(const uint8_t *byteArray, uint32_t len);
		
		void write(const char *format, ...);
		
	private:
		MainUart(void);
		
};





#endif
