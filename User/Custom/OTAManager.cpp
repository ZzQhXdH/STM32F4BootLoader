#include "Custom/OTAManager.h"
#include "Driver/MainUart.h"

#define OTA_MESSAGE_ADDRESS		(9 * 4 * 1024)
#define OTA_ADDRESS				(10 * 4 * 1024)
#define SECTOR_SIZE				(4 * 1024)
#define APP_ADDRESS				(0x08004000)

uint64_t mAppBuffer[SECTOR_SIZE / 8];

OTAManager::OTAManager(void)
{
	
}

bool OTAManager::isUpdate(void)
{
	uint8_t buf[4];
	FlashMemory::instance().vReadData(OTA_MESSAGE_ADDRESS, (uint8_t *) &buf, 4);
	mPackageSize = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
	uint8_t buffer[6] = { 0xE1, buf[0], buf[1], buf[2], buf[3], 0xEF };
	MainUart::instance().write(buffer, 6);
	return (mPackageSize > 0) && (mPackageSize <= 400 * 1024);
}

void OTAManager::update(void)
{
	if (!isUpdate())
	{
		return;
	}
	InteriorFlash::instance().vEarseApplicationSector(mPackageSize);
	
	for (uint32_t i = 0; i < mPackageSize; i += SECTOR_SIZE)
	{
		FlashMemory::instance().vReadData(OTA_ADDRESS + i, (uint8_t *) mAppBuffer, SECTOR_SIZE);
		InteriorFlash::instance().vWriteBuffer(APP_ADDRESS + i, (uint32_t *) mAppBuffer, SECTOR_SIZE / 4);
	}
	FlashMemory::instance().vSectorEarse(OTA_MESSAGE_ADDRESS);
	isUpdate();
}
