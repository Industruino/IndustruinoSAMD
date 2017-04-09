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

#include "EthernetReset.h"
#include <Reset.h>


void EthernetReset::stdResponse (char *msg)
{
   ethernetClient.println("HTTP/1.1 200 OK");
   ethernetClient.println("Content-Type: text/html");
   ethernetClient.println("Connnection: close");
   ethernetClient.println();
   ethernetClient.println("<!DOCTYPE HTML>");
   ethernetClient.println("<html>");
   ethernetClient.println(msg);
   ethernetClient.println("</html>");
}

void EthernetReset::notFoundResponse (char *msg)
{
   ethernetClient.println("HTTP/1.1 404 OK");
   ethernetClient.println("Content-Type: text/html");
   ethernetClient.println("Connnection: close");
   ethernetClient.println();
   ethernetClient.println("<!DOCTYPE HTML>");
   ethernetClient.println("<html>");
   ethernetClient.println(msg);
   ethernetClient.println("</html>");
}

void EthernetReset::reset (void)
{
   delay(10UL);
   ethernetClient.stop();
   immediateReset();
}

EthernetReset::EthernetReset (int port, char *p)
{
   ethernetServer = new EthernetServer(port);

   strncpy(path, p, PATH_MAX_LENGTH);
}

void EthernetReset::begin (uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
   Ethernet.begin(mac_address, local_ip, dns_server, gateway, subnet);
   ethernetServer->begin();
}

#define HTTP_REQ_PREFIX_LENGTH     5   // "GET /"
#define HTTP_SLASH_LENGTH          1   // "/"
#define HTTP_REQ_POSTFIX_LENGTH    9   // " HTTP/1.1"
#define MAX_COMMAND_LENGTH        10

#define REQUEST_BUFFER_EXTRA_SIZE (HTTP_REQ_PREFIX_LENGTH + HTTP_SLASH_LENGTH + MAX_COMMAND_LENGTH + HTTP_REQ_POSTFIX_LENGTH)

void EthernetReset::check (void)
{
   char http_req[strlen(path) + REQUEST_BUFFER_EXTRA_SIZE];
   ethernetClient = ethernetServer->available();

   if (ethernetClient)
   {
      while (ethernetClient.connected())
      {
         if (ethernetClient.available())
         {
            char c;
            char *url = http_req;

            while ((c = ethernetClient.read()) != '\n')
               if (url < (http_req + strlen(path) + REQUEST_BUFFER_EXTRA_SIZE))
                  *(url++) = c;

            url = http_req + HTTP_REQ_PREFIX_LENGTH;
            ethernetClient.flush();

            if (!strncmp(url, path, strlen(path)))
            {
               url += (strlen(path) + HTTP_SLASH_LENGTH);

               if (!strncmp(url, "reset", 5))
               {
                  stdResponse("Rebooting...");
                  reset();
               }
               else
                  stdResponse("Wrong command");
            }
            else
               notFoundResponse("Unknown path");

            break;
         }
      }

      delay(10UL);
      ethernetClient.stop();
	}
}
