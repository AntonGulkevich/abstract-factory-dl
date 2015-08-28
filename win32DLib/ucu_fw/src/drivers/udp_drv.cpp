/*
 * udp.c
 *
 *  Created on: 09.04.2014
 *      Author: Admin
 */

//#define UDP
#ifdef UDP

#include "udp_drv.h"

#include "xparameters.h"
#include "xil_printf.h"
#include "netif/xadapter.h"
#include "lwip/init.h"
#include "lwip/udp.h"

#define PLATFORM_EMAC_BASEADDR XPAR_XEMACPS_0_BASEADDR

struct netif main_netif_inst;
struct netif *main_netif;

void tcp_fasttmr(void);
void tcp_slowtmr(void);

#define RESET_RX_CNTR_LIMIT	400
#ifndef USE_SOFTETH_ON_ZYNQ
static int ResetRxCntr = 0;
extern struct netif *main_netif;
#endif

#define PBUF_SIZE 1024

//struct udp_pcb *udp_receiver;
//struct udp_pcb *udp_sender;

//udp_drv_callback receiver_handler = 0;
//void* receiver_context = (void *)0;

void udp_drv_init(unsigned int  ipaddr, unsigned int  netmask, unsigned int  gw, unsigned char* mac_ethernet_address)
{
	main_netif = &main_netif_inst;
	lwip_init();
	if (!xemac_add(main_netif,
			(ip_addr_t*)(&ipaddr), (ip_addr_t*)(&netmask), (ip_addr_t*)(&gw),
			mac_ethernet_address,
			PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		return;
	}

	netif_set_default(main_netif);
}

void udp_drv_up()
{
	netif_set_up(main_netif);
}

void udp_drv_on_poll()
{
	xemacif_input(main_netif);
}

void udp_drv_on_timer()
{
	/* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified
	 * by lwIP. It is not important that the timing is absoluetly accurate.
	 */
	static int odd = 1;

	odd = !odd;
#ifndef USE_SOFTETH_ON_ZYNQ
	ResetRxCntr++;
#endif
	/*tcp_fasttmr();
	if (odd) {
		tcp_slowtmr();
	}*/

	/* For providing an SW alternative for the SI #692601. Under heavy
	 * Rx traffic if at some point the Rx path becomes unresponsive, the
	 * following API call will ensures a SW reset of the Rx path. The
	 * API xemacpsif_resetrx_on_no_rxdata is called every 100 milliseconds.
	 * This ensures that if the above HW bug is hit, in the worst case,
	 * the Rx path cannot become unresponsive for more than 100
	 * milliseconds.
	 */
#ifndef USE_SOFTETH_ON_ZYNQ
	if (ResetRxCntr >= RESET_RX_CNTR_LIMIT) {
		xemacpsif_resetrx_on_no_rxdata(main_netif);
		ResetRxCntr = 0;
	}
#endif

}


void udp_drv_send(struct udp_pcb *udp_sender, void* data, unsigned int size)
{

	struct pbuf* sp = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_POOL);

	sp->flags = 1;
	sp->len = size;
	sp->tot_len = sp->len + (sp->next? sp->next->tot_len: 0);
	memcpy(sp->payload, data, size);
	udp_send(udp_sender, sp);
	pbuf_free(sp);
}

void udp_recv_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

	udp_drv_callback receiver_handler = (udp_drv_callback)arg;

	if(receiver_handler != 0)
		receiver_handler(p->payload, p->len);

	pbuf_free(p);
}

struct udp_pcb *udp_drv_sender_register(ip_addr_t* destination_ip, unsigned short port)
{
	struct udp_pcb *udp_sender = udp_new();
	udp_connect(udp_sender, destination_ip, (u16_t)port);
	return udp_sender;
}

struct udp_pcb *udp_drv_receive_register(unsigned short port, udp_drv_callback handler)
{

	struct udp_pcb *udp_receiver = udp_new();
	udp_bind(udp_receiver, IP_ADDR_ANY, (u16_t)port);
	udp_recv(udp_receiver, udp_recv_callback, (void*)handler);
	return udp_receiver;

}

#endif
