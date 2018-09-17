#ifndef		__LOG_UART_H
#define		__LOG_UART_H

#include "stm32f4xx.h"



class LogUart
{
	public:
		static inline LogUart &instance(void) {
			static LogUart uart;
			return uart;
		}
	
		void write(uint8_t byte);
		
		void write(const char *format, ...);
		
		void write(const uint8_t *byteArray, uint32_t len);
		
		inline void set(const void *addr, uint32_t len)
		{
			receiver = (const uint8_t *) addr;
			length = len;
			isReceiver = true;
		}
		
		inline const uint8_t *data(void) {
			return receiver;
		}
		
		inline uint32_t size() { return length; }
		
		bool isReceiver;
		
	private:
		LogUart(void);
	
	private:
		const uint8_t *receiver;
		uint32_t length;
		
};


#endif
