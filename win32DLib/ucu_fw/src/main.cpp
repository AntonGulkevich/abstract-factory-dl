/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 *
 *
 * This file is a generated sample test application.
 *
 * This application is intended to test and/or illustrate some
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * SDK application project when you run the "Generate Libraries" menu item.
 *
 */

#include <stdio.h>
#include <cstdio>
#include "xparameters.h"
//#include "xil_cache.h"
//#include "xil_exception.h"
//#include "xuartps.h"

#include "application/cconfiguration.h"

#include "drivermanager.h"
#include "interfaces/iusingflags.h"
#include "drivers/usbtr.h"
#include "drivers/qspi.h"
#include "drivers/display.h"
#include "drivers/irqmanager.h"
#include "drivers/gpio.h"
#include "utilities/fwloader.h"

#include "drivers/gpio.h"
#include "drivers/i2c.h"

#include "drivers/keys.h"

#include "driversiohw/adccontrol.h"

#include "types.h"
#include "driversiohw/analogin.h"
#include "driversiohw/arincin.h"
#include "driversiohw/arincout.h"
#include "driversiohw/arincwordout.h"
#include "driversiohw/nptout.h"
#include "driversiohw/potentiometr.h"
#include "driversiohw/rsout.h"
#include "driversiohw/sctout.h"
#include "driversiomanager.h"
#include "flagmanager.h"


#include "drivers/termometr.h"
#include "utilities/diagnostic.h"
#include "utilities/commod.h"
#include "utilities/console.h"
#include "workmanager.h"


/*
#include "lwip/udp.h"
#include "drivers/udp_drv.h"
#include "lwip/ip_addr.h"

struct udp_pcb* udp_sender1;
struct udp_pcb* udp_sender2;
struct udp_pcb* udp_receiver1;
struct udp_pcb* udp_receiver2;

#define inet_addr(a,b,c,d)    ( ((unsigned int )a) | ((unsigned int )b << 8) |\
                                ((unsigned int )c << 16) | ((unsigned int )d << 24) )

// ::: ��������� ��������� :::

// ������ ������� � �������������, ������������� �� 10 �� 1000.
#define TIMER_PERIOD 100
// IP-�����
#define IP_ADDRESS	(inet_addr(172,  22,   14, 100))
// ����� ����
#define NET_MASK 	(inet_addr(255,   0,    0,   0))
// ���� �� ���������
#define GATEWAY 	(inet_addr(172,  22,   22, 254))
// ������� (MAC) �����
unsigned char MAC_ADDRESS[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };


// ���������� �� ������ ������ �� UDP
void udp_handler1(void* data, unsigned int size)
{
	char vvv[1000];
	sprintf(vvv, "������� ������ �� UDP, ���� %d: %s",(int)12345 , data);
}

// ���������� �� ������ ������ �� UDP
void udp_handler2(void* data, unsigned int size)
{
	char vvv[1000];
	sprintf(vvv, "������� ������ �� UDP, ���� %d: %s",(int)12346 , data);

}

*/

int main() {

#ifndef EMULATION


	Xil_ICacheEnable();
	Xil_DCacheEnable();

#endif

/*

	DriverManager drivers;
	drivers.EnableEthernet(false);

	ip_addr_t dest_ip;

	// ������������� �������� ����������
			udp_drv_init(IP_ADDRESS, NET_MASK, GATEWAY, MAC_ADDRESS);
	// ��� ����� ��� �������� ������ �� UDP
		IP4_ADDR(&dest_ip, 172,22,14,1);
		// ��������� ������ ������ �� UDP
		udp_sender1 = udp_drv_sender_register(&dest_ip, 12345);
		// ��������� ������ ������ �� UDP
		udp_sender2 = udp_drv_sender_register(&dest_ip, 12346);


		// ��������� ������ ������ �� UDP
		udp_receiver1 = udp_drv_receive_register(12345, udp_handler1);
		// ��������� ������ ������ �� UDP
		udp_receiver2 = udp_drv_receive_register(12346, udp_handler2);

		// ������ ���������� Ethernet
		udp_drv_up();




		while(1)
		{
			char buff[1000];
			// ������ �������� �� UDP
			sprintf(buff, "Sended to port %d\r\n", (int)12345);
			int len =  strlen(buff);
			udp_drv_send(udp_sender1, buff, len);

			sprintf(buff, "Sended to port %d\r\n", (int)12346);
			len =  strlen(buff);
			udp_drv_send(udp_sender2, buff, len);

			udp_drv_on_poll();
		}

*/
	DriverManager driverManager;
	driverManager.GetDisplay()->SetTextByText("3A  ");
	Gpio::SVSet();

	DriversIOManager driversIO;
	//driverManager.GetGpio()->SetLED(Gpio::LEDS::L4_1, driversIO.GetADCControl()->IsInternalIrqEnabled());
	driverManager.GetDisplay()->SetTextByText("3A� ");
	Gpio::SVSet();
	WorkManager workManager(&driversIO, &driverManager);
	driverManager.GetDisplay()->SetTextByText("3A��");
	Gpio::SVSet();

	if (driverManager.GetClock()->IsHotStart())
	{
		Console::TraceLine("������� �����");
		driverManager.GetGpio()->SetLED(Gpio::LEDS::L7_1, true);
	}
	else
		Console::TraceLine("�������� �����");

	if (driverManager.GetI2CCommod()->IsDeviceAvailable() || driverManager.GetKeys()->IsBreakCombinationPressed())
	{
		workManager.Init();
	}
	else
	{
		driverManager.GetDisplay()->SetTextByText("A200");
		for(;;);
	}
	driverManager.EnableAllIrq();

	UINT tickCount = driverManager.GetPerfomance()->GetValue();
	for(;;)
	{
/*		driverManager.GetGpio()->SetLED(Gpio::LEDS::L4_1, driversIO.GetADCControl()->GetState().iostSync1Fail == 1);
		driverManager.GetGpio()->SetLED(Gpio::LEDS::L4_2, driversIO.GetADCControl()->GetState().iostSync2Fail == 1);
		driverManager.GetGpio()->SetLED(Gpio::LEDS::L4_3, driversIO.GetADCControl()->GetState().iostSync3Fail == 1);
		driverManager.GetGpio()->SetLED(Gpio::LEDS::L4_4, driversIO.GetADCControl()->GetState().iostSync4Fail == 1);*/

		driverManager.GetFlagManager()->Process();
		if (driverManager.GetPerfomance()->GetValue()-tickCount > 40 * 50000)
		{
			tickCount = driverManager.GetPerfomance()->GetValue();
			if (!driversIO.IsIrqNormal())
				driverManager.GetDisplay()->SetTextByText("�000", true);
		}
		driverManager.GetGpio()->SetLED(Gpio::LEDS::L4_3, UCU_IORD_32DIRECT(XPAR_MAIN_ADC_HUB_0_BASEADDR, 0x1034) == 0);

	}

#ifndef EMULATION
	Xil_DCacheDisable();
	Xil_ICacheDisable();
#endif
	return 0;
}
