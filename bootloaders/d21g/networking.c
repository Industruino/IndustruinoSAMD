//  Basic networking functions
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
//     Author: .
//     Copyright: Arduino
//     License: GPL http://www.gnu.org/licenses/gpl-2.0.html
//     Project: eboot
//     Function: Network initialization
//     Version: 0.1 tftp / flashing functional

#include "networking.h"
#include "fram.h"
#include "netcfg.h"
#include "sdcard.h"
#include "spi.h"
#include "w5x00.h"

#if ETHERNET_CHIP == WIZNET_W5100
#include "utils.h"
#endif


void netInit (void)
{
   spiInit(SPI_BITRATE);

   sdcardInit();
   framInit();

#if W5X00_USE_HARDWARE_RESET
   w5x00Init();
   sdCardSetSpiMode();
#else
   sdCardSetSpiMode();
   w5x00Init();
#endif

   netcfgInit();
}

bool netIsReady (void)
{
   return w5x00IsReady();
}

bool netConfigure (void)
{
   if (w5x00Configure(netcfgData.macAddr,
                      netcfgData.ipAddr,
                      netcfgData.networkMask,
                      netcfgData.gatewayAddr))
      return true;

   spiEnd();

   return false;
}

void netEnd (void)
{
   w5x00End();
   netcfgEnd();

   spiEnd();
}

void netOpenSocket3 (void)
{
   // Open Socket
   w5x00WriteReg(REG_S3_CR, S3_W_CB, CR_OPEN);

   // Wait for command to complete
   while (w5x00ReadReg(REG_S3_CR, S3_R_CB));
}

void netCloseSocket3 (void)
{
   // Close the socket
   w5x00WriteReg(REG_S3_CR, S3_W_CB, CR_CLOSE);

   // Wait for command to complete
   while (w5x00ReadReg(REG_S3_CR, S3_R_CB));
}

void netInitUdpSocket3 (uint16_t port)
{
   do
   {
      netCloseSocket3();

      // Clear the socket interrupt register
      w5x00WriteReg(REG_S3_IR, S3_W_CB, 0xFF);
      // Set UDP mode
      w5x00WriteReg(REG_S3_MR, S3_W_CB, MR_UDP);
      // Set the socket port
      w5x00WriteWord(REG_S3_PORT0, S3_W_CB, port);

      netOpenSocket3();
   }
   // Continue only if socket is correctly opened
   while (w5x00ReadReg(REG_S3_SR, S3_R_CB) != SOCK_UDP);
}

uint16_t netReceivedDataSizeSocket3 (void)
{
   return w5x00ReadWord(REG_S3_RX_RSR0, S3_R_CB);
}

//TODO What's the purpose of this function?  To be investigated...
void netSync (uint32_t msDelay)
{
   while ((w5x00ReadReg(REG_S3_IR, S3_R_CB) & IR_RECV))
   {
      w5x00WriteReg(REG_S3_IR, S3_W_CB, IR_RECV);
#if ETHERNET_CHIP == WIZNET_W5100
      //FIXME: is this right after all? smaller delay but
      //still a delay and it still breaks occasionally
      delay(msDelay);
#endif
   }
}

void netSendPacketSocket3 (uint8_t *txPtr, uint16_t packetSize)
{
#if ETHERNET_CHIP == WIZNET_W5500
   uint16_t writePointer = w5x00ReadWord(REG_S3_TX_WR0, S3_R_CB);
#else
   uint16_t writePointer = w5x00ReadWord(REG_S3_TX_WR0, 0) + S3_TX_START;
#endif

   while (packetSize--)
   {
      w5x00WriteReg(writePointer++, S3_TXBUF_CB, *txPtr++);
#if ETHERNET_CHIP == WIZNET_W5500
      // W5500 auto increments the readpointer by memory mapping a 16 bit address
      // Use uint16_t overflow from 0xFFFF to 0x10000 to follow W5500 internal pointer
   }
   w5x00WriteWord(REG_S3_TX_WR0, S3_W_CB, writePointer);
#else
      if (writePointer == S3_TX_END)
          writePointer = S3_TX_START;
   }
   w5x00WriteWord(REG_S3_TX_WR0, S3_W_CB, writePointer - S3_TX_START);
#endif

   // Transmit the data
   w5x00WriteReg(REG_S3_CR, S3_W_CB, CR_SEND);

   // Wait for transmission to complete
   while (w5x00ReadReg(REG_S3_CR, S3_R_CB));
}

void netReceivePacketSocket3 (uint8_t* buffer, uint16_t packetSize)
{
   uint8_t *bufPtr = buffer;
   uint16_t count,
            readPointer;

   // Read data from chip to buffer
   readPointer = w5x00ReadWord(REG_S3_RX_RD0, S3_R_CB);

#if ETHERNET_CHIP == WIZNET_W5500
   // W5500 auto increments the readpointer by memory mapping a 16 bit address
#else
   if (readPointer == 0)
      readPointer += S3_RX_START;
#endif

   for (count = packetSize; count--;)
   {
#if ETHERNET_CHIP == WIZNET_W5500
      *bufPtr++ = w5x00ReadReg(readPointer++, S3_RXBUF_CB);
      // W5500 auto increments the readpointer by memory mapping a 16 bit address
      // Use uint16_t overflow from 0xFFFF to 0x10000 to follow W5500 internal pointer
#else
      *bufPtr++ = w5x00ReadReg(readPointer++, 0);
      if (readPointer == S3_RX_END)
         readPointer = S3_RX_START;
#endif
   }

   // Send the new pointer value back to the chip
   w5x00WriteWord(REG_S3_RX_RD0, S3_W_CB, readPointer);

   w5x00WriteReg(REG_S3_CR, S3_W_CB, CR_RECV);

   while (w5x00ReadReg(REG_S3_CR, S3_R_CB));

   // Set up return IP address and port
   uint8_t i;

   for (i = 0; i < 6; ++i)
      w5x00WriteReg(REG_S3_DIPR0 + i, S3_W_CB, buffer[i]);
}
