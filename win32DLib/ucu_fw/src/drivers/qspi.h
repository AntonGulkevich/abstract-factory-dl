/*
 * qspi.h
 *
 *  Created on: 16 марта 2015 г.
 *      Author: Youbrin_A
 */

#ifndef QSPI_H_
#define QSPI_H_

#include "../types.h"

//#include "xqspips_hw.h"
//#include "xqspips.h"



#define WRITE_STATUS_CMD	0x01
#define WRITE_CMD		0x02
#define READ_CMD		0x03
#define WRITE_DISABLE_CMD	0x04
#define READ_STATUS_CMD		0x05
#define WRITE_ENABLE_CMD	0x06
#define FAST_READ_CMD		0x0B
#define DUAL_READ_CMD		0x3B
#define QUAD_READ_CMD		0x6B
#define BULK_ERASE_CMD		0xC7
#define	SEC_ERASE_CMD		0xD8
#define READ_ID			0x9F



class Qspi
{
private:
	const static DWORD DATA_SIZE = 4096;
	const static DWORD DATA_SIZE_LOG = 12;

	const static DWORD SECTOR_SIZE_LOG = 16;
	const static DWORD SECTOR_SIZE = (1 << SECTOR_SIZE_LOG);
	const static DWORD SECTOR_MASK = (SECTOR_SIZE-1);
	const static DWORD NUM_SECTORS = 0x100;
	const static DWORD NUM_PAGES = 0x10000;
	const static DWORD PAGE_SIZE_LOG = 8;
	const static DWORD PAGE_SIZE = (1 << PAGE_SIZE_LOG);
	const static DWORD PAGE_MASK = (PAGE_SIZE-1);

	const static DWORD COMMAND_OFFSET = 0; /* FLASH instruction */
	const static DWORD ADDRESS_1_OFFSET = 1; /* MSB byte of address to read or write */
	const static DWORD ADDRESS_2_OFFSET = 2; /* Middle byte of address to read or write */
	const static DWORD ADDRESS_3_OFFSET = 3; /* LSB byte of address to read or write */
	const static DWORD DATA_OFFSET = 4; /* Start of Data for Read/Write */
	const static DWORD DUMMY_OFFSET = 4; /* Dummy byte offset for fast, dual and quad reads */
	const static DWORD DUMMY_SIZE = 1; /* Number of dummy bytes for fast, dual and quad reads */
	const static DWORD RD_ID_SIZE = 4; /* Read ID command + 3 bytes ID response */
	const static DWORD BULK_ERASE_SIZE = 1; /* Bulk Erase command size */
	const static DWORD SEC_ERASE_SIZE = 4; /* Sector Erase command + Sector address */

	DWORD _deviceId;

	//u8 _readBuffer[DATA_SIZE + DATA_OFFSET + DUMMY_SIZE];
	//u8 _writeBuffer[DATA_OFFSET + PAGE_SIZE + DUMMY_SIZE];

	//XQspiPs  _qspiInstance;
	//XQspiPs* _qspiInstancePtr;

	//// Заполняются во FlashReadID
	//u32 QspiFlashSize;
	//u32 QspiFlashMake;

	//u32 FlashReadID();
	void ReadPage(void* buffer, int offset, int address, int count);
	void WritePage(void* buffer, int offset, int address, int count);
public:
	void Init();

	Qspi(DWORD deviceId);
	virtual ~Qspi();

	void Read(void* buffer, UINT address, UINT count);
	void Write(void* buffer, UINT address, UINT count);

	void Erase(UINT address, UINT count);

};

#endif /* QSPI_H_ */
