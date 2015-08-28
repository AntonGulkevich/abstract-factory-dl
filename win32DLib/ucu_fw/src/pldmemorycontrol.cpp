/*
 * pldmemorycontrol.cpp
 *
 *  Created on: 12 рту. 2015 у.
 *      Author: Youbrin_A
 */

#include "pldmemorycontrol.h"

PldMemoryControl memControl;


DWORD PldMemoryControl::UcuRead(UINT baseAddress, UINT offset)
{
	TestAddress(baseAddress + offset);
	return (*(volatile DWORD*)((baseAddress) + (offset)));
}
float PldMemoryControl::UcuReadF(UINT baseAddress, UINT offset)
{
	TestAddress(baseAddress + offset);
	return (*(volatile float*)((baseAddress) + (offset)));
}
void PldMemoryControl::UcuWrite(UINT baseAddress, UINT offset, DWORD data)
{
	TestAddress(baseAddress + offset);
	(*(volatile DWORD*)((baseAddress) + (offset))) = data;
}
void PldMemoryControl::UcuWriteF(UINT baseAddress, UINT offset, float data)
{
	TestAddress(baseAddress + offset);
	(*(volatile float*)((baseAddress) + (offset))) = data;
}

bool PldMemoryControl::TestAddress(DWORD address)
{
	if ((address & 0x3) != 0)
		return false;
	for(int i = 0; i < AddressCount-1; i++)
	{
		if (address > Addresses[i][1] && address < Addresses[i+1][0])
			return false;
	}
	return true;
}

DWORD PldMemoryControl::Addresses[PldMemoryControl::AddressCount][2] =
{
{0x0, 0x3ffff}, //PS7_RAM_0_S_AXI_BASE
{0x100000, 0xfffffff}, //PS7_DDR_0_S_AXI_BASE
{0x43c00000, 0x43c0ffff}, //ARINC_IN_TOP_0_BASE
{0x43c10000, 0x43c1ffff}, //ARINC_IN_TOP_1_BASE
{0x43c20000, 0x43c2ffff}, //ARINC_IN_TOP_2_BASE
{0x43c30000, 0x43c3ffff}, //ARINC_IN_TOP_3_BASE
{0x43c40000, 0x43c4ffff}, //ARINC_IN_TOP_4_BASE
{0x43c50000, 0x43c5ffff}, //ARINC_IN_TOP_5_BASE
{0x43c60000, 0x43c6ffff}, //ARINC_OUT_TOP_0_BASE
{0x43c70000, 0x43c7ffff}, //ARINC_OUT_TOP_1_BASE
{0x43c80000, 0x43c8ffff}, //ARINC_OUT_TOP_2_BASE
{0x43c90000, 0x43c9ffff}, //ARINC_OUT_TOP_3_BASE
{0x43ca0000, 0x43caffff}, //ARINC_TABLE_0_BASE
{0x43cb0000, 0x43cbffff}, //TEST_ARINC_0_BASE
{0x43cc0000, 0x43ccffff}, //CURRENT_DAC_TOP_0_BASE
{0x43cd0000, 0x43cdffff}, //EX_MAX_0_BASE
{0x43ce0000, 0x43ceffff}, //INFO_RS_0_BASE
{0x43cf0000, 0x43cfffff}, //MAIN_ADC_HUB_0_BASE
{0x43d00000, 0x43d0ffff}, //POT_TOP_0_BASE
{0x43d10000, 0x43d1ffff}, //RESOLV_TOP_0_BASE
{0x43d20000, 0x43d2ffff}, //NAND_FLASH_TOP_0_BASE
{0x43d30000, 0x43d3ffff}, //RS485_TOP_0_BASE
{0x43d40000, 0x43d4ffff}, //TOP_USB_0_BASE
{0xe0000000, 0xe0000fff}, //XUARTPS_0_BASE
{0xe0000000, 0xe0000fff}, //PS7_UART_0_BASE
{0xe0004000, 0xe0004fff}, //PS7_I2C_0_BASE
{0xe0004000, 0xe0004fff}, //XIICPS_0_BASE
{0xe0005000, 0xe0005fff}, //XIICPS_1_BASE
{0xe0005000, 0xe0005fff}, //PS7_I2C_1_BASE
{0xe000a000, 0xe000afff}, //XGPIOPS_0_BASE
{0xe000a000, 0xe000afff}, //PS7_GPIO_0_BASE
{0xe000b000, 0xe000bfff}, //XEMACPS_0_BASE
{0xe000b000, 0xe000bfff}, //PS7_ETHERNET_0_BASE
{0xe000d000, 0xe000dfff}, //PS7_QSPI_0_BASE
{0xe000d000, 0xe000dfff}, //XQSPIPS_0_BASE
{0xe0200000, 0xe0200fff}, //PS7_IOP_BUS_CONFIG_0_S_AXI_BASE
{0xf8000000, 0xf8000fff}, //PS7_SLCR_0_S_AXI_BASE
{0xf8003000, 0xf8003fff}, //PS7_DMA_S_BASE
{0xf8003000, 0xf8003fff}, //XDMAPS_1_BASE
{0xf8004000, 0xf8004fff}, //PS7_DMA_NS_BASE
{0xf8004000, 0xf8004fff}, //XDMAPS_0_BASE
{0xf8006000, 0xf8006fff}, //PS7_DDRC_0_S_AXI_BASE
{0xf8007000, 0xf80070ff}, //PS7_DEV_CFG_0_BASE
{0xf8007000, 0xf80070ff}, //XDCFG_0_BASE
{0xf8007100, 0xf8007120}, //XADCPS_0_BASE
{0xf8007100, 0xf8007120}, //PS7_XADC_0_BASE
{0xf8008000, 0xf8008fff}, //PS7_AFI_0_S_AXI_BASE
{0xf8009000, 0xf8009fff}, //PS7_AFI_1_S_AXI_BASE
{0xf800a000, 0xf800afff}, //PS7_AFI_2_S_AXI_BASE
{0xf800b000, 0xf800bfff}, //PS7_AFI_3_S_AXI_BASE
{0xf800c000, 0xf800cfff}, //PS7_OCMC_0_S_AXI_BASE
{0xf8800000, 0xf88fffff}, //PS7_CORESIGHT_COMP_0_S_AXI_BASE
{0xf8891000, 0xf8891fff}, //PS7_PMU_0_S_AXI_BASE
{0xf8893000, 0xf8893fff}, //PS7_PMU_0_PMU1_S_AXI_BASE
{0xf8900000, 0xf89fffff}, //PS7_GPV_0_S_AXI_BASE
{0xf8f00000, 0xf8f000fc}, //PS7_SCUC_0_S_AXI_BASE
{0xf8f00100, 0xf8f001ff}, //PS7_SCUGIC_0_BASE
{0xf8f00100, 0xf8f001ff}, //SCUGIC_0_CPU_BASE
{0xf8f00200, 0xf8f002ff}, //PS7_GLOBALTIMER_0_S_AXI_BASE
{0xf8f00600, 0xf8f0061f}, //XSCUTIMER_0_BASE
{0xf8f00600, 0xf8f0061f}, //PS7_SCUTIMER_0_BASE
{0xf8f00620, 0xf8f006ff}, //PS7_SCUWDT_0_BASE
{0xf8f00620, 0xf8f006ff}, //SCUWDT_0_BASE
{0xf8f01000, 0xf8f01000}, //SCUGIC_0_DIST_BASE
{0xf8f01000, 0xf8f01fff}, //PS7_INTC_DIST_0_S_AXI_BASE
{0xf8f01000, 0xf8f01000}, //PS7_SCUGIC_0_DIST_BASE
{0xf8f02000, 0xf8f02fff}, //PS7_PL310_0_S_AXI_BASE
{0xf8f02000, 0xf8f02fff}, //PS7_L2CACHEC_0_S_AXI_BASE
{0xfc000000, 0xfcffffff}, //PS7_QSPI_LINEAR_0_S_AXI_BASE
{0xfffc0000, 0xffffffff}, //PS7_RAM_1_S_AXI_BASE
};

PldMemoryControl::PldMemoryControl() {
	// TODO Auto-generated constructor stub

}

PldMemoryControl::~PldMemoryControl() {
	// TODO Auto-generated destructor stub
}

