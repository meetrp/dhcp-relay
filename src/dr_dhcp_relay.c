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
 * @file				dr_dhcp_relay.c
 *
 * @brief 				This is the file where the DHCP relay entry point is defined.
 *
 * @date				07-Jun-2016 2:42:21 pm
 * @author			rp (rp@meetrp.com)
 */

#include <stdio.h>										/* fprintf */
#include <unistd.h>										/* close */
#include <ev.h>											/* libev */

#include <arpa/inet.h>									/* htons */
#include <linux/if_packet.h>						/* sockaddr_ll */
#include <net/if.h>										/* ifreq */
#include <netinet/if_ether.h>						/* ETH_P_ALL */
#include <sys/ioctl.h>									/* SIOCGIFINDEX */
#include <sys/socket.h>								/* socket, AF_PACKET, SOCK_RAW */

#include "dr_common.h"
#include "dr_dhcp_relay.h"

PRIVATE dr_err_t __bind_to_interface(IN int sock_fd, IN char* dev)
{
	int interface_idx = 0;
	struct ifreq interface_request;
	struct sockaddr_ll sock_addr =
	{
		0
	};
	const int opt = 1;

	memset(&interface_request, 0, sizeof(struct ifreq));
	strncpy(interface_request.ifr_name, dev, IFNAMSIZ);

	/* get the current lags the device */
	if (ioctl(sock_fd, SIOCGIFFLAGS, &interface_request) == -1) {
		fprintf(stderr, "Unable to get the interface index!\n");
		return DR_IOCTL_FAILED;
	}

	/* set the old flags with promiscuous mode */
	interface_request.ifr_flags |= IFF_PROMISC;
	if (ioctl(sock_fd, SIOCSIFFLAGS, &interface_request) == -1) {
		fprintf(stderr, "Unable to set the promiscuous mode!\n");
		return DR_IOCTL_FAILED;
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) == -1) {
		fprintf(stderr, "Unable to set the reuse address!\n");
		return DR_SET_SOCK_OPT_FAILED;
	}

	/* configure the device */
	if (ioctl(sock_fd, SIOCGIFINDEX, &interface_request) == -1) {
		fprintf(stderr, "Unable to get the interface index!\n");
		return DR_IOCTL_FAILED;
	}
	interface_idx = interface_request.ifr_ifindex;
	fprintf(stderr, "Index: %d\n", interface_idx);

	sock_addr.sll_family = AF_PACKET;
	sock_addr.sll_protocol = htons(ETH_P_ALL);
	sock_addr.sll_ifindex = interface_idx;

	if (bind(sock_fd, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0) {
		printf("Unable to bind!");
		return DR_BIND_FAILED;
	}

	return DR_SUCCESS;
}

PRIVATE void __fwd(IN unsigned char* buf, IN char* dest_mac,
	IN unsigned int sport, IN int vlan_flag, IN int len, IN unsigned char mac_last)
{
	int sock_out = -1;
	int data_len = len;

	struct sockaddr_ll sock_addr;
	struct ifreq if_req;

	unsigned char data_buf[MAX_BUF_SIZE];
	memset(data_buf, 0, MAX_BUF_SIZE);

	if ((sock_out = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP))) < 0) {
		fprintf(stderr, "unable to open socket!\n");
		return;
	}

	if (sport == 68) { /* from client to server */
		if (mac_last == 0xaa) {
			strncpy(if_req.ifr_name, EGRESS_DEV_3, IFNAMSIZ);
			if (ioctl(sock_out, SIOCGIFINDEX, &if_req) == -1) {
				fprintf(stderr, "Unable to get interface index: %s\n", EGRESS_DEV_100);
				close(sock_out);
				return;
			}
		}
		else {
			strncpy(if_req.ifr_name, EGRESS_DEV_100, IFNAMSIZ);
			if (ioctl(sock_out, SIOCGIFINDEX, &if_req) == -1) {
				fprintf(stderr, "Unable to get interface index: %s\n", EGRESS_DEV_3);
				close(sock_out);
				return;
			}
		}
	}
	else if (sport == 67) { /* from server back to client */
		strncpy(if_req.ifr_name, INGRESS_DEV, IFNAMSIZ);
		if (ioctl(sock_out, SIOCGIFINDEX, &if_req) == -1) {
			fprintf(stderr, "Unable to get interface index: %s\n", INGRESS_DEV);
			close(sock_out);
			return;
		}
	}

	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sll_family = AF_PACKET;
	sock_addr.sll_protocol = htons(ETH_P_IP);
	sock_addr.sll_ifindex = if_req.ifr_ifindex;
	sock_addr.sll_halen = ETHER_ADDR_LEN;
	sock_addr.sll_addr[0] = dest_mac[0];
	sock_addr.sll_addr[1] = dest_mac[1];
	sock_addr.sll_addr[2] = dest_mac[2];
	sock_addr.sll_addr[3] = dest_mac[3];
	sock_addr.sll_addr[4] = dest_mac[4];
	sock_addr.sll_addr[5] = dest_mac[5];

	if ((vlan_flag == 1) && (sport == 67)) { /* from vlan interface to outside */
		int i = ETHER_HDR_LEN - ETHER_TYPE_LEN;
		fprintf(stderr, "%d %d", len, i);
		memcpy(data_buf, buf, i);

		fprintf(stderr, "%d %d", len, len - i);
		memcpy(data_buf + i, buf + i + 4, len - i);

		data_len -= 4; /* VLAN tag */
	}
	else {
		memcpy(data_buf, buf, data_len);
	}

	if (sendto(sock_out, data_buf, data_len, 0, (struct sockaddr*) &sock_addr,
			sizeof(struct sockaddr_ll)) < 0) {
		fprintf(stderr, "Unable to send\n");
	}

	close(sock_out);
}

PRIVATE void __cb(EV_P_ ev_io *w, int revents)
{
	unsigned char buf[MAX_BUF_SIZE];
	struct sockaddr_ll saddr;
	socklen_t saddr_size = sizeof(saddr);
	dr_eth_hdr_t *ehdr = NULL;
	int len = 0;
	int vlan_id = 0;
	dr_ip_hdr_t *iphdr = NULL;
	dr_udp_hdr_t *udphdr = NULL;
	struct in_addr _blah;
	char *src = NULL;
	char *dst = NULL;
	unsigned char* dhcp_data = NULL;
	int offset = 0;

	if ((len = recvfrom(w->fd, buf, MAX_BUF_SIZE, 0, (struct sockaddr*)&saddr,
				(socklen_t*) &saddr_size)) < 0) {
		return;
	}

	ehdr = (dr_eth_hdr_t*) buf;
	if (ehdr->type != ETH_IP) {
		return;
	}

	if (ehdr->type == ETH_VLAN_IP) {
		iphdr = (dr_ip_hdr_t*) (buf + sizeof(dr_eth_hdr_vlan_t));
		offset += sizeof(dr_eth_hdr_vlan_t);
		vlan_id = 1;
	}
	else {
		iphdr = (dr_ip_hdr_t*) (buf + sizeof(dr_eth_hdr_t));
		offset += sizeof(dr_eth_hdr_t);
	}

	if (iphdr->proto != IP_UDP) {
		return;
	}
	offset += sizeof(dr_ip_hdr_t);

	udphdr = (dr_udp_hdr_t*) (buf + sizeof(dr_eth_hdr_t) + sizeof(dr_ip_hdr_t));
	if ((ntohs(udphdr->dport) != 67) && (ntohs(udphdr->dport) != 68))
	{
		return;
	}
	offset += sizeof(dr_udp_hdr_t);

	switch(saddr.sll_pkttype) {
		case PACKET_HOST:
		fprintf(stderr, "LOCAL HOST\n");
		break;
		case PACKET_OTHERHOST:
		fprintf(stderr, "OTHER HOST\n");
		break;
		case PACKET_BROADCAST:
		fprintf(stderr, "BROADCAST\n");
		break;
		case PACKET_MULTICAST:
		fprintf(stderr, "MULTICAST\n");
		break;
		case PACKET_OUTGOING:
		fprintf(stderr, "OUTGOING\n");
		break;
	}

	if ((saddr.sll_pkttype == PACKET_OUTGOING) && (ntohs(udphdr->dport) == 67)) {
		return;
	}

	_blah.s_addr = iphdr->src;
	src = strdup(inet_ntoa(_blah));
	_blah.s_addr = iphdr->dst;
	dst = strdup(inet_ntoa(_blah));

	dhcp_data = (unsigned char*) (buf + offset);

	fprintf(stderr, "    IP: %s[%u] --> %s:%u\n", src, ntohs(udphdr->sport),
		dst, ntohs(udphdr->dport));
	if (dhcp_data[0] == 1) {
		fprintf(stderr, "    OP: %d (REQUEST)\n", dhcp_data[0]);
	}
	else {
		fprintf(stderr, "  OP: %d (REPLY)\n", dhcp_data[0]);
	}
	fprintf(stderr, "   XID: %02x%02x%02x%02x\n", dhcp_data[4], dhcp_data[5],
		dhcp_data[6], dhcp_data[7]);
	fprintf(stderr, "CIADDR: %d.%d.%d.%d\n", dhcp_data[12], dhcp_data[13],
		dhcp_data[14], dhcp_data[15]);
	fprintf(stderr, "YIADDR: %d.%d.%d.%d\n", dhcp_data[16], dhcp_data[17],
		dhcp_data[18], dhcp_data[19]);
	fprintf(stderr, "SIADDR: %d.%d.%d.%d\n", dhcp_data[20], dhcp_data[21],
		dhcp_data[22], dhcp_data[23]);
	fprintf(stderr, "GIADDR: %d.%d.%d.%d\n", dhcp_data[24], dhcp_data[25],
		dhcp_data[26], dhcp_data[27]);
	fprintf(stderr, "CHADDR: %02x%02x%02x%02x%02x%02x\n", dhcp_data[28],
		dhcp_data[29], dhcp_data[30], dhcp_data[31], dhcp_data[32],
		dhcp_data[33]);

	__fwd(buf, dst, ntohs(udphdr->sport), vlan_id, len, dhcp_data[33]);

	fprintf(stderr, "--------------------------------------------------\n");
}

PUBLIC dr_err_t dr_start_dhcp_relay(IN char* dev)
{
	dr_err_t ret = DR_SUCCESS;

	int raw_sock = -1;
	ev_io dhcp_watcher;

	if ((raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		fprintf(stderr, "Socket error!\n");
		return DR_SOCKET_FAILURE;
	}

	if ((ret = __bind_to_interface(raw_sock, dev)) != DR_SUCCESS) {
		fprintf(stderr, "Bind failed!\n");
		return ret;
	}

	fprintf(stderr, "about to init watcher\n");
	ev_io_init(&dhcp_watcher, __cb, raw_sock, EV_READ);
	ev_io_start(EV_DEFAULT, &dhcp_watcher);
	ev_loop(EV_DEFAULT, 0);

	close(raw_sock);
	return DR_SUCCESS;
}
