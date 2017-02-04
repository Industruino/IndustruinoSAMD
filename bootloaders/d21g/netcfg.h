//  Network configuration data
//  Copyright (C) 2017  Industruino <connect@industruino.com>
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Developed by Claudio Indellicati <bitron.it@gmail.com>

#ifndef __NETCFG_H__
#define __NETCFG_H__

#include <sam.h>
#include "board_definitions.h"

#define NETCFG_CHECKSUM_SIZE       1
#define NETCFG_CHECKSUM_ADDR       ((uint16_t)(FRAM_SIZE-NETCFG_CHECKSUM_SIZE))
#define NETCFG_DATA_REV_SIZE       1
#define NETCFG_DATA_REV_ADDR       ((uint16_t)(NETCFG_CHECKSUM_ADDR-NETCFG_DATA_REV_SIZE))
#define NETCFG_MAC_ADDR_SIZE       (sizeof(MACAddr))
#define NETCFG_MAC_ADDR_ADDR       ((uint16_t)(NETCFG_DATA_REV_ADDR-NETCFG_MAC_ADDR_SIZE))
#define NETCFG_IP_ADDR_SIZE        (sizeof(IPAddr))
#define NETCFG_IP_ADDR_ADDR        ((uint16_t)(NETCFG_MAC_ADDR_ADDR-NETCFG_IP_ADDR_SIZE))
#define NETCFG_NETWORK_MASK_SIZE   NETCFG_IP_ADDR_SIZE
#define NETCFG_NETWORK_MASK_ADDR   ((uint16_t)(NETCFG_IP_ADDR_ADDR-NETCFG_NETWORK_MASK_SIZE))
#define NETCFG_GATEWAY_ADDR_SIZE   NETCFG_IP_ADDR_SIZE
#define NETCFG_GATEWAY_ADDR_ADDR   ((uint16_t)(NETCFG_NETWORK_MASK_ADDR-NETCFG_GATEWAY_ADDR_SIZE))
#define NETCFG_TFTP_DATA_PORT_SIZE (sizeof(uint16_t))
#define NETCFG_TFTP_DATA_PORT_ADDR ((uint16_t)(NETCFG_GATEWAY_ADDR_ADDR-NETCFG_TFTP_DATA_PORT_SIZE))
#define NETCFG_PASSWORD_SIZE       8
#define NETCFG_PASSWORD_ADDR       ((uint16_t)(NETCFG_TFTP_DATA_PORT_ADDR-NETCFG_PASSWORD_SIZE))

#define NETCFG_NET_CONFIG_DATA_SIZE ((uint16_t)(FRAM_SIZE-NETCFG_PASSWORD_ADDR))
#define NETCFG_NET_CONFIG_DATA_ADDR ((uint16_t)(FRAM_SIZE-NETCFG_NET_CONFIG_DATA_SIZE))


typedef uint8_t MACAddr[6];
typedef uint8_t IPAddr[4];

typedef union _NetConfigData
{
  uint8_t rawData[NETCFG_NET_CONFIG_DATA_SIZE];
  struct
  {
    char password[NETCFG_PASSWORD_SIZE];
    uint16_t tftpDataPort;
    IPAddr gatewayAddr;
    IPAddr networkMask;
    IPAddr ipAddr;
    MACAddr macAddr;
    uint8_t revision;
    uint8_t checksum;
  };
} NetConfigData;

extern NetConfigData netcfgData;

void netcfgInit (void);
void netcfgEnd (void);


#endif   // __NETCFG_H__
