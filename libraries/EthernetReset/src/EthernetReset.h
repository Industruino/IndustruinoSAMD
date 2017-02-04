//  Library to reset the SAMD21 MCU via Ethernet
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
//
//  Code adapted from Ariadne bootloader (https://github.com/codebendercc/Ariadne-Bootloader)
//  Original copyright:
//     Copyright (c) 2012 Stylianos Tsampas.  All right reserved.

#ifndef __ETHERNETRESET_H__
#define __ETHERNETRESET_H__

#include <Arduino.h>
#include <IPAddress.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetServer.h>
#include <EthernetClient.h>

#define pgm_uchar(name) static const prog_uchar name[]

#define PATH_SIZE       (20)
#define PATH_MAX_LENGTH (PATH_SIZE - 1)

class EthernetReset
{
   private:

      EthernetServer *ethernetServer;
      EthernetClient ethernetClient;
      char path[PATH_SIZE];

      void stdResponse (char *msg);
      void notFoundResponse (char *msg);
      void reset (void);
      void stop (void);

   public:

      EthernetReset (int port, char *p);

      void begin (uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);
      void check (void);
};

#endif   // __ETHERNETRESET_H__
