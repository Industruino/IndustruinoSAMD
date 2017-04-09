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

#include <stdbool.h>
#include "netcfg.h"
#include "fram.h"

#define NETCFG_CONFIG_DATA_REVISION ((uint8_t)(1))

NetConfigData defaultNetConfig =
{ .rawData =
  {
    NETCFG_DEFAULT_PASSWORD,
    (NETCFG_DEFAULT_TFTP_DATA_PORT & 0xFF),
    (NETCFG_DEFAULT_TFTP_DATA_PORT >> 8),
    NETCFG_DEFAULT_GATEWAY_ADDR,
    NETCFG_DEFAULT_NETWORK_MASK,
    NETCFG_DEFAULT_IP_ADDR,
    NETCFG_DEFAULT_MAC_ADDR,
    NETCFG_CONFIG_DATA_REVISION,
    0U
  }
};

NetConfigData netcfgData;

static bool netcfgCheckData (NetConfigData *ncd)
{
   if (ncd->revision != NETCFG_CONFIG_DATA_REVISION)
      return false;

   for (uint8_t i = 0U; i < NETCFG_PASSWORD_SIZE; ++i)
      if ((ncd->password[i] < ' ') || (ncd->password[i] > 0x7E))
         return false;

   uint8_t checksum = 0U;

   for (uint16_t i = 0U; i < (NETCFG_NET_CONFIG_DATA_SIZE - NETCFG_CHECKSUM_SIZE); ++i)
      checksum += ncd->rawData[i];

   if (checksum != ncd->checksum)
      return false;

   return true;
}

static bool netcfgReadDataFromMemory (NetConfigData *ncd)
{
   return framRead(NETCFG_NET_CONFIG_DATA_ADDR, ncd->rawData, NETCFG_NET_CONFIG_DATA_SIZE);
}

static void netcfgSetChecksum (NetConfigData *ncd)
{
  ncd->checksum = 0U;

  for (uint16_t i = 0U; i < (NETCFG_NET_CONFIG_DATA_SIZE - NETCFG_CHECKSUM_SIZE); ++i)
    ncd->checksum += ncd->rawData[i];
}

void netcfgInit (void)
{
   framInit();

   netcfgReadDataFromMemory(&netcfgData);

   if (netcfgCheckData(&netcfgData))
      return;

   for (uint16_t i = 0U; i < (NETCFG_NET_CONFIG_DATA_SIZE - NETCFG_CHECKSUM_SIZE); ++i)
      netcfgData.rawData[i] = defaultNetConfig.rawData[i];

   netcfgSetChecksum(&netcfgData);
}

void netcfgEnd (void)
{
   framEnd();
}
