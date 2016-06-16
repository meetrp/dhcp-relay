/*************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Rp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *************************************************************/

/**
 * @file 				dr_dhcp_relay.h
 *
 * @brief 				File contains all the necessary definitions for dhcp relay to be called
 * All the function prototypes and the data structure required to utilize the dhcp
 * relay functions will be defined in this file.
 *
 * @date 				07-Jun-2016 2:42:21 pm
 * @author			rp (rp@meetrp.com)
 */

#ifndef DR_DHCP_RELAY_H_
#define DR_DHCP_RELAY_H_

#include "dr_common.h"

#define MAX_BUF_SIZE		65536		///< Maximum buffer size

#define INGRESS_DEV				"eth1"		///< Interface exposed to clients
#define EGRESS_DEV_100			"eth1.100"	///< Interface on which DHCP listens on
#define EGRESS_DEV_3				"eth1.3"	///< Interface on which DHCP listens on

/**
 * @brief Ethernet protocol header
 */
typedef struct __eth_hdr {
		unsigned char dmac[6];					///< destination MAC
		unsigned char smac[6];					///< source MAC
		unsigned short type;						///< ethernet type
#define ETH_IP				8						///< IP protocol
#define ETH_VLAN_IP	129					///< VLAN IPv4
} dr_eth_hdr_t;

/**
 * @brief Ethernet protocol header (with VLAN ID)
 */
typedef struct __eth_hdr_vlan {
		unsigned char dmac[6];					///< destination MAC
		unsigned char smac[6];					///< source MAC
		unsigned short type;						///< ethernet type
		unsigned vlan_prio :3;						///< vlan prio
		unsigned vlan_CFI :1;						///< vlan canonical form id
		unsigned vlan_id :12;						///< vlan id
		unsigned short en_type;					///< encapsulated ethernet type
} dr_eth_hdr_vlan_t;

/**
 * @brief IP protocol header
 */
typedef struct __ip_hdr {
#ifdef LITTLE_ENDIAN
		unsigned char ver :4, hdr_len :4;		///< version & IP hdr len
#else
		unsigned char hdr_len :4, ver :4;		///< IP hdr len & version
#endif
		unsigned char tos;							///< service type
		unsigned short int len;						///< total length
		unsigned short int id;						///< identification
		unsigned short int off;						///< flag & offset
		unsigned char ttl;							///< time to live
		unsigned char proto;						///< protocol
#define IP_TCP		6								///< TCP packet
#define IP_UDP		17							///< UDP packet
		unsigned short int checksum;			///< header checksum
		unsigned int src;								///< source IP address
		unsigned int dst;							///< destination IP address
}__attribute__ ((packed)) dr_ip_hdr_t;

/**
 * @brief UDP packet header
 */
typedef struct __udp_hdr {
		unsigned short int sport;					///< Source port
		unsigned short int dport;					///< Destination port
		unsigned short int length;				///< total length
		unsigned short int checksum;			///< checksum
} dr_udp_hdr_t;

/**
 * @brief start the DHCP relay loop
 * This function starts the DHCP relay on the given network interface ('dev')
 *
 * @param [in] dev		network interface on which relay should run
 * @return error codes
 */
PUBLIC dr_err_t dr_start_dhcp_relay(IN char* dev);

#endif /* DR_DHCP_RELAY_H_ */
