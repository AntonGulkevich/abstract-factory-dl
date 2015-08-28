/*
 * qspi.cpp
 *
 *  Created on: 16 марта 2015 г.
 *      Author: Youbrin_A
 */

#include "qspi.h"
//#include "xil_printf.h"

#include "gpio.h"


#define SINGLE_FLASH_CONNECTION			0
#define DUAL_STACK_CONNECTION			1
#define DUAL_PARALLEL_CONNECTION		2
#define FLASH_SIZE_16MB					0x1000000

/*
 * Bank mask
 */
#define BANKMASK 0xF000000

/*
 * Identification of Flash
 * Micron:
 * Byte 0 is Manufacturer ID;
 * Byte 1 is first byte of Device ID - 0xBB or 0xBA
 * Byte 2 is second byte of Device ID describes flash size:
 * 128Mbit : 0x18; 256Mbit : 0x19; 512Mbit : 0x20
 * Spansion:
 * Byte 0 is Manufacturer ID;
 * Byte 1 is Device ID - Memory Interface type - 0x20 or 0x02
 * Byte 2 is second byte of Device ID describes flash size:
 * 128Mbit : 0x18; 256Mbit : 0x19; 512Mbit : 0x20
 */

#define MICRON_ID		0x20
#define SPANSION_ID		0x01
#define WINBOND_ID		0xEF

#define FLASH_SIZE_ID_128M		0x18
#define FLASH_SIZE_ID_256M		0x19
#define FLASH_SIZE_ID_512M		0x20
#define FLASH_SIZE_ID_1G		0x21

/*
 * Size in bytes
 */
#define FLASH_SIZE_128M			0x1000000
#define FLASH_SIZE_256M			0x2000000
#define FLASH_SIZE_512M			0x4000000
#define FLASH_SIZE_1G			0x8000000

/*
 * The following constants define the commands which may be sent to the FLASH
 * device.
 */
#define OVERHEAD_SIZE		4

/*
 * The following constants specify the max amount of data and the size of the
 * the buffer required to hold the data and overhead to transfer the data to
 * and from the FLASH.
 */


/******************************************************************************
*
* This function reads serial FLASH ID connected to the SPI interface.
* It then deduces the make and size of the flash and obtains the
* connection mode to point to corresponding parameters in the flash
* configuration table. The flash driver will function based on this and
* it presently supports Micron and Spansion - 128, 256 and 512Mbit and
* Winbond 128Mbit
*
* @param	none
*
* @return	XST_SUCCESS if read id, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
//u32 Qspi::FlashReadID()
//{
//	u32 Status;
//
//	/*
//	 * Read ID in Auto mode.
//	 */
//	_writeBuffer[COMMAND_OFFSET]   = READ_ID;
//	_writeBuffer[ADDRESS_1_OFFSET] = 0x00;		/* 3 dummy bytes */
//	_writeBuffer[ADDRESS_2_OFFSET] = 0x00;
//	_writeBuffer[ADDRESS_3_OFFSET] = 0x00;
//
//	Status = XQspiPs_PolledTransfer(_qspiInstancePtr, _writeBuffer, _readBuffer,
//				RD_ID_SIZE);
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//
//	xil_printf("Single Flash Information\r\n");
//
//	xil_printf("FlashID=0x%x 0x%x 0x%x\r\n", _readBuffer[1],
//			_readBuffer[2],
//			_readBuffer[3]);
//
//	/*
//	 * Deduce flash make
//	 */
//	if (_readBuffer[1] == MICRON_ID) {
//		QspiFlashMake = MICRON_ID;
//		xil_printf("MICRON ");
//	} else if(_readBuffer[1] == SPANSION_ID) {
//		QspiFlashMake = SPANSION_ID;
//		xil_printf("SPANSION ");
//	} else if(_readBuffer[1] == WINBOND_ID) {
//		QspiFlashMake = WINBOND_ID;
//		xil_printf("WINBOND ");
//	}
//
//	/*
//	 * Deduce flash Size
//	 */
//	if (_readBuffer[3] == FLASH_SIZE_ID_128M) {
//		QspiFlashSize = FLASH_SIZE_128M;
//		xil_printf("128M Bits\r\n");
//	} else if (_readBuffer[3] == FLASH_SIZE_ID_256M) {
//		QspiFlashSize = FLASH_SIZE_256M;
//		xil_printf("256M Bits\r\n");
//	} else if (_readBuffer[3] == FLASH_SIZE_ID_512M) {
//		QspiFlashSize = FLASH_SIZE_512M;
//		xil_printf("512M Bits\r\n");
//	} else if (_readBuffer[3] == FLASH_SIZE_ID_1G) {
//		QspiFlashSize = FLASH_SIZE_1G;
//		xil_printf("1G Bits\r\n");
//	}
//
//	return XST_SUCCESS;
//}


Qspi::Qspi(DWORD deviceId) : _deviceId(deviceId)
{
	//_qspiInstancePtr = &_qspiInstance;
}

Qspi::~Qspi()
{
}

void Qspi::Init()
{
//	int Status;
//	XQspiPs_Config *QspiConfig;
//
//
//	/*
//	 * Initialize the QSPI driver so that it's ready to use
//	 */
//	QspiConfig = XQspiPs_LookupConfig(_deviceId);
//	if (NULL == QspiConfig) {
//		return;
//	}
//
//	Status = XQspiPs_CfgInitialize(_qspiInstancePtr, QspiConfig,
//					QspiConfig->BaseAddress);
//	if (Status != XST_SUCCESS) {
//		return;
//	}
//
//	/*
//	 * Perform a self-test to check hardware build
//	 */
//	Status = XQspiPs_SelfTest(_qspiInstancePtr);
//	if (Status != XST_SUCCESS) {
//		return;
//	}
//
//	/*
//	 * Set Manual Start and Manual Chip select options and drive HOLD_B
//	 * pin high.
//	 */
//	XQspiPs_SetOptions(_qspiInstancePtr, XQSPIPS_MANUAL_START_OPTION |
//			XQSPIPS_FORCE_SSELECT_OPTION |
//			XQSPIPS_HOLD_B_DRIVE_OPTION);
//
//	/*
//	 * Set the prescaler for QSPI clock
//	 */
//	XQspiPs_SetClkPrescaler(_qspiInstancePtr, XQSPIPS_CLK_PRESCALE_8);
//
//	/*
//	 * Assert the FLASH chip select.
//	 */
//	XQspiPs_SetSlaveSelect(_qspiInstancePtr);
//
//
//	FlashReadID();
}


void Qspi::ReadPage(void* buffer, int offset, int address, int count)
{
//	UINT ByteCount = count;
//
//	_writeBuffer[COMMAND_OFFSET]   = READ_CMD;
//	_writeBuffer[ADDRESS_1_OFFSET] = (u8)((address & 0xFF0000) >> 16);
//	_writeBuffer[ADDRESS_2_OFFSET] = (u8)((address & 0xFF00) >> 8);
//	_writeBuffer[ADDRESS_3_OFFSET] = (u8)(address & 0xFF);
//
//	/*if ((Command == FAST_READ_CMD) || (Command == DUAL_READ_CMD) ||
//	    (Command == QUAD_READ_CMD)) {
//		ByteCount += DUMMY_SIZE;
//	}*/
//
//	XQspiPs_PolledTransfer(_qspiInstancePtr, _writeBuffer, _readBuffer,
//				ByteCount + OVERHEAD_SIZE);
//	memcpy(&reinterpret_cast<u8*>(buffer)[offset], &_readBuffer[DATA_OFFSET], count);
}

void Qspi::Read(void* buffer, UINT address, UINT count)
{
	UINT offset = 0;
	while(count > DATA_SIZE)
	{
		ReadPage(buffer, offset, address, DATA_SIZE);
		count -= DATA_SIZE;
		offset += DATA_SIZE;
		address += DATA_SIZE;
	}
	if(count > 0)
		ReadPage(buffer, offset, address, count);
}

void Qspi::WritePage(void* buffer, int offset, int address, int count)
{
//	Gpio::SVSet();
//	memcpy(&_writeBuffer[4], &reinterpret_cast<u8*>(buffer)[offset], count);
//	UINT ByteCount = count;
//	u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
//	u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };  /* must send 2 bytes */
//	u8 FlashStatus[2];
//
//	XQspiPs_PolledTransfer(_qspiInstancePtr, &WriteEnableCmd, NULL,
//				sizeof(WriteEnableCmd));
//
//	_writeBuffer[COMMAND_OFFSET]   = WRITE_CMD;
//	_writeBuffer[ADDRESS_1_OFFSET] = (u8)((address & 0xFF0000) >> 16);
//	_writeBuffer[ADDRESS_2_OFFSET] = (u8)((address & 0xFF00) >> 8);
//	_writeBuffer[ADDRESS_3_OFFSET] = (u8)(address & 0xFF);
//
//	XQspiPs_PolledTransfer(_qspiInstancePtr, _writeBuffer, NULL, ByteCount + OVERHEAD_SIZE);
//
//	while (1) {
//		XQspiPs_PolledTransfer(_qspiInstancePtr, ReadStatusCmd, FlashStatus, sizeof(ReadStatusCmd));
//		if ((FlashStatus[1] & 0x01) == 0)
//			break;
//	}
}

void Qspi::Write(void* buffer, UINT address, UINT count)
{
	// Первая страница
	UINT offset = 0;
	UINT firstCount = PAGE_SIZE - (address & PAGE_MASK);
	if (firstCount >= count) // Единственная страница
	{
		WritePage(buffer, 0, address, count);
	}
	else
	{
		WritePage(buffer, 0, address, firstCount);
		count -= firstCount;
		address += firstCount;
		offset += firstCount;
		while (count >= PAGE_SIZE)
		{
			WritePage(buffer, offset, address, PAGE_SIZE);
			count -= PAGE_SIZE;
			address += PAGE_SIZE;
			offset += PAGE_SIZE;
		}
		if (count > 0)
		{
			WritePage(buffer, offset, address, count);
		}
	}
}

void Qspi::Erase(UINT address, UINT count)
{
//	u8 WriteEnableCmd = { WRITE_ENABLE_CMD };
//	u8 ReadStatusCmd[] = { READ_STATUS_CMD, 0 };  /* must send 2 bytes */
//	u8 FlashStatus[2];
//
//	/*
//	 * If erase size is same as the total size of the flash, use bulk erase
//	 * command
//	 */
//	if (count == (NUM_SECTORS * SECTOR_SIZE))
//	{
//		/*
//		 * Send the write enable command to the FLASH so that it can be
//		 * written to, this needs to be sent as a seperate transfer
//		 * before the erase
//		 */
//		XQspiPs_PolledTransfer(_qspiInstancePtr, &WriteEnableCmd, NULL,
//				  sizeof(WriteEnableCmd));
//
//		/*
//		 * Setup the bulk erase command
//		 */
//		_writeBuffer[COMMAND_OFFSET]   = BULK_ERASE_CMD;
//
//		/*
//		 * Send the bulk erase command; no receive buffer is specified
//		 * since there is nothing to receive
//		 */
//		XQspiPs_PolledTransfer(_qspiInstancePtr, _writeBuffer, NULL, BULK_ERASE_SIZE);
//
//		/*
//		 * Wait for the erase command to the FLASH to be completed
//		 */
//		while (1)
//		{
//			/*
//			 * Poll the status register of the device to determine
//			 * when it completes, by sending a read status command
//			 * and receiving the status byte
//			 */
//			XQspiPs_PolledTransfer(_qspiInstancePtr, ReadStatusCmd, FlashStatus, sizeof(ReadStatusCmd));
//
//			/*
//			 * If the status indicates the write is done, then stop
//			 * waiting; if a value of 0xFF in the status byte is
//			 * read from the device and this loop never exits, the
//			 * device slave select is possibly incorrect such that
//			 * the device status is not being read
//			 */
//			if ((FlashStatus[1] & 0x01) == 0)
//				break;
//		}
//
//		return;
//	}
//
//	/*
//	 * If the erase size is less than the total size of the flash, use
//	 * sector erase command
//	 */
//	UINT startSector = (address >> SECTOR_SIZE_LOG);
//	UINT stopSector = ((address + count) >> SECTOR_SIZE_LOG);
//	if ((stopSector << SECTOR_SIZE_LOG) == (address + count))
//		stopSector--;
//	for (UINT sector = startSector; sector <= stopSector; sector++)
//	{
//		/*
//		 * Send the write enable command to the SEEPOM so that it can be
//		 * written to, this needs to be sent as a seperate transfer
//		 * before the write
//		 */
//		XQspiPs_PolledTransfer(_qspiInstancePtr, &WriteEnableCmd, NULL,
//					sizeof(WriteEnableCmd));
//
//		/*
//		 * Setup the write command with the specified address and data
//		 * for the FLASH
//		 */
//		UINT addr = sector << SECTOR_SIZE_LOG;
//		_writeBuffer[COMMAND_OFFSET]   = SEC_ERASE_CMD;
//		_writeBuffer[ADDRESS_1_OFFSET] = (u8)(addr >> 16);
//		_writeBuffer[ADDRESS_2_OFFSET] = (u8)(addr >> 8);
//		_writeBuffer[ADDRESS_3_OFFSET] = (u8)(addr & 0xFF);
//
//		/*
//		 * Send the sector erase command and address; no receive buffer
//		 * is specified since there is nothing to receive
//		 */
//		XQspiPs_PolledTransfer(_qspiInstancePtr, _writeBuffer, NULL,
//					SEC_ERASE_SIZE);
//
//		/*
//		 * Wait for the sector erse command to the FLASH to be completed
//		 */
//		while (1)
//		{
//			/*
//			 * Poll the status register of the device to determine
//			 * when it completes, by sending a read status command
//			 * and receiving the status byte
//			 */
//			XQspiPs_PolledTransfer(_qspiInstancePtr, ReadStatusCmd,
//						FlashStatus,
//						sizeof(ReadStatusCmd));
//
//			/*
//			 * If the status indicates the write is done, then stop
//			 * waiting, if a value of 0xFF in the status byte is
//			 * read from the device and this loop never exits, the
//			 * device slave select is possibly incorrect such that
//			 * the device status is not being read
//			 */
//			if ((FlashStatus[1] & 0x01) == 0)
//				break;
//		}
//	}
}


