/*
 * udp.h
 *
 *  Created on: 09.04.2014
 *      Author: Admin
 */

#ifndef UDP_H_
#define UDP_H_

#include "lwip/udp.h"
#include "lwip/ip_addr.h"

typedef void(*udp_drv_callback)(void*, unsigned int size);

void udp_drv_init(unsigned int ipaddr, unsigned int netmask, unsigned int gw, unsigned char* mac_ethernet_address);
void udp_drv_up();
void udp_drv_on_poll();
void udp_drv_on_timer();

void udp_drv_send(struct udp_pcb *udp_sender, void* data, unsigned int size);
struct udp_pcb *udp_drv_sender_register(ip_addr_t* destination_ip, unsigned short port);
struct udp_pcb *udp_drv_receive_register(unsigned short port, udp_drv_callback handler);

#endif /* UDP_H_ */
