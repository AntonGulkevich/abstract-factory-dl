/*
 * fwloader.cpp
 *
 *  Created on: 23 марта 2015 г.
 *      Author: Youbrin_A
 */

#include "fwloader.h"

#include "m_math.h"
#include "../drivers/qspi.h"
#include "../drivers/display.h"
#include "../drivers/usbtr.h"
#include <cstdio>



FWLoader::FWLoader(Qspi* qspi, Display* display, UsbTr* usb) : _qspi(qspi), _display(display), _usb(usb)
{


}

FWLoader::~FWLoader()
{

}

void FWLoader::LoadFW()
{

	const UINT PACKET_SIZE = 64*1024;

	UINT currentPacket = 0;
	// Wait
	_display->SetTextByText("Load", true);
	_usb->ResetStatus(UsbTr::All);
	_usb->SetTimeout(1000);
	UsbTr::STATUS_MASK status = _usb->WaitForStatus(UsbTr::All);
	if (!(status & UsbTr::SomeError))
	{
		UINT totalPacket = _usb->GetTotalPacketCount();
		BYTE* data = new BYTE[PACKET_SIZE * totalPacket];
		bool isOk = true;
		CRC32 crc32;
		while(totalPacket > currentPacket)
		{
			status = _usb->WaitForStatus(UsbTr::Data64Received);
			if (!(status == UsbTr::SomeError))
			{
				_usb->ResetStatus(UsbTr::Data64Received);
				UINT cp = _usb->GetCurrentPacketNumber();
				_display->SetTextByValue(totalPacket-cp, false);
				UINT crc = _usb->GetCRC();
				if (currentPacket == cp)
				{
					crc32.Reset();
					_usb->Receive64kData(&data[currentPacket * PACKET_SIZE]);
					crc32.ProcessCRC(&data[currentPacket * PACKET_SIZE], PACKET_SIZE);
					if (crc != crc32.GetCRC32())
					{
						_usb->SendAnswer(UsbTr::WrongCrc);
					} else
					{
						currentPacket++;
						_usb->SendAnswer(UsbTr::Correct);
					}
				}
				else
				{
					_usb->SendAnswer(UsbTr::WrongPacketNumber);
					isOk = false;
					break;
				}
			}
			else
			{
				_usb->ResetStatus(status);
				_usb->SendAnswer(UsbTr::Break);
				isOk = false;
				break;
			}

		}

		if (isOk && totalPacket > 0)
		{
			for(UINT i = 0; i < totalPacket; i++)
			{
				char buff[5];
				sprintf(buff, "C %02d", totalPacket - i);
				_display->SetTextByText(buff);
				_qspi->Erase(i * PACKET_SIZE, PACKET_SIZE);
				_usb->SendProgress(1, i);
				Gpio::SVSet();
			}
			_display->SetTextByText("3AП ");
			_usb->SendProgress(2, 0);
			_qspi->Write(data, 0, totalPacket * PACKET_SIZE);
			_usb->SendProgress(3, 0);
			_display->SetTextByText("УСП ");
			for(;;);
		} else
		{
			_display->SetTextByText("Err");
			_usb->SendProgress(0, 0);
		}

		delete[] data;
	}
	else
	{
		_usb->ResetStatus(status);
		_usb->SendAnswer(UsbTr::Break);
		_display->SetTextByText("Err ");
	}
	_usb->SetTimeout(20);
}






