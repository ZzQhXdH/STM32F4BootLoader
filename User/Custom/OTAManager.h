#ifndef		__OTA_MANAGER_H
#define		__OTA_MANAGER_H

#include "Driver/FlashMemory.h"
#include "Driver/InteriorFlash.h"


class OTAManager
{
	public:
		static inline OTAManager &instance(void) {
			static OTAManager m;
			return m;
		}
		
		bool isUpdate(void);
		void update(void);
	
	private:
		OTAManager(void);
	
	private:
		uint32_t mPackageSize;
};



#endif
