//  Minimal Wiznet W5x00 driver
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
//
//  Wiznet W5500 differentiation code adapted from https://github.com/jbkim/Differentiate-WIznet-Chip

#include "w5x00.h"
#include "spi.h"

#define W5X00_READ  (0x0F)
#define W5X00_WRITE (0xF0)

#define W5X00_GW_ADDR_POS  1
#define W5X00_NETMASK_POS  5
#define W5X00_MAC_ADDR_POS 9
#define W5X00_IP_ADDR_POS  15

#define W5X00_REGISTER_BLOCK_SIZE 28
uint8_t registerBuffer[W5X00_REGISTER_BLOCK_SIZE] =
{
   0x00,               // MR Mode

   0, 0, 0, 0,         // GWR Gateway IP Address Register
   0, 0, 0, 0,         // SUBR Subnet Mask Register
   0, 0, 0, 0, 0, 0,   // SHAR Source Hardware Address Register
   0, 0, 0, 0,         // SIPR Source IP Address Register

#if ETHERNET_CHIP == WIZNET_W5100
   0, 0,         // Reserved locations
   0,            // IR Interrupt Register
   0,            // IMR Interrupt Mask Register
   0x07, 0xD0,   // RTR Retry Time-value Register
   0x80,         // RCR Retry Count Register
   0x55,         // RMSR Rx Memory Size Register, 2K per socket
   0x55          // TMSR Tx Memory Size Register, 2K per socket
#elif ETHERNET_CHIP == WIZNET_W5500
   0, 0,         // Interrupt Low Level Timer (INTLEVEL0), (INTLEVEL1) (0x0013, 0x0014)
   0,            // IR Interrupt Register (0x0015)
   0,            // IMR Interrupt Mask Register (0x0016)
   0,            // Socket Interrupt (SIR) (0x0017)
   0,            // Socket Interrupt Mask (SIMR) (0x0018)
   0x07, 0xD0,   // RTR Retry Time-value Register ((RTR0),(RTR0)) (0x0019,0x001A)
   0x08          // RCR Retry Count Register (0x001B)
#endif
};

#define W5X00_INIT_CS     PORT->Group[W5X00_CS_PORT].DIRSET.reg=(1<<W5X00_CS_PIN)
#define W5X00_END_CS      PORT->Group[W5X00_CS_PORT].DIRCLR.reg=(1<<W5X00_CS_PIN)
#define W5X00_ASSERT_CS   PORT->Group[W5X00_CS_PORT].OUTCLR.reg=(1<<W5X00_CS_PIN)
#define W5X00_DEASSERT_CS PORT->Group[W5X00_CS_PORT].OUTSET.reg=(1<<W5X00_CS_PIN)

uint64_t w5x00ResetEndTime = 0xFFFFFFFFFFFFFFFF;


void w5x00Reset()
{
#if W5X00_USE_HARDWARE_RESET

   //
   //  Hardware reset
   //

   PORT->Group[W5X00_RESET_PORT].DIRSET.reg = (1<<W5X00_RESET_PIN);
#if W5X00_RESET_LEVEL_HIGH
   PORT->Group[W5X00_RESET_PORT].OUTCLR.reg = (1<<W5X00_RESET_PIN);
   delay(2UL);
   PORT->Group[W5X00_RESET_PORT].OUTSET.reg = (1<<W5X00_RESET_PIN);
   delay(1UL);
   PORT->Group[W5X00_RESET_PORT].OUTCLR.reg = (1<<W5X00_RESET_PIN);
   delay(2UL);
#else   // W5X00_RESET_LEVEL_HIGH
   PORT->Group[W5X00_RESET_PORT].OUTSET.reg = (1<<W5X00_RESET_PIN);
   delay(2UL);
   PORT->Group[W5X00_RESET_PORT].OUTCLR.reg = (1<<W5X00_RESET_PIN);
   delay(1UL);
   PORT->Group[W5X00_RESET_PORT].OUTSET.reg = (1<<W5X00_RESET_PIN);
   delay(2UL);
#endif   // W5X00_RESET_LEVEL_HIGH

#else   // W5X00_USE_HARDWARE_RESET

   //
   //  Software reset
   //

#if ETHERNET_CHIP == WIZNET_W5500
   w5x00WriteReg(0, 0x04, 0x80);
#else
   w5x00WriteReg(0, 0, 0x80);
#endif

#endif   // W5X00_USE_HARDWARE_RESET

   w5x00ResetEndTime = millis() + W5X00_POST_RESET_DELAY;
}

void w5x00Init()
{
   W5X00_INIT_CS;

   w5x00Reset();
}

bool w5x00IsReady (void)
{
   return (millis() > w5x00ResetEndTime);
}

bool w5x00Configure (uint8_t macAddr[], uint8_t ipAddr[], uint8_t netMask[], uint8_t gwAddr[])
{
   bool chipAvailable = true;

   W5X00_ASSERT_CS;

#if ETHERNET_CHIP == WIZNET_W5100
   if (spiTransfer(0x0F) != 0x00) chipAvailable = false;   // OP-Code Field - Read operation
   if (spiTransfer(0x00) != 0x01) chipAvailable = false;   // Address
   if (spiTransfer(0x00) != 0x02) chipAvailable = false;   // Address
   spiTransfer(0x00);
#endif

#if ETHERNET_CHIP == WIZNET_W5500
   // Adapted form https://github.com/jbkim/Differentiate-WIznet-Chip
   spiTransfer(0x00);
   spiTransfer(0x39);
   spiTransfer(0x00);
   chipAvailable = (spiTransfer(0x00) == 0x04);
#endif

   W5X00_DEASSERT_CS;

  if (!chipAvailable)
    return false;

   uint8_t idx;

   for (idx = 0; idx < 4; ++idx)
   {
      registerBuffer[idx + W5X00_IP_ADDR_POS] = ipAddr[idx];
      registerBuffer[idx + W5X00_NETMASK_POS] = netMask[idx];
      registerBuffer[idx + W5X00_GW_ADDR_POS] = gwAddr[idx];
   }

   for (idx = 0; idx < 6; ++idx)
      registerBuffer[idx + W5X00_MAC_ADDR_POS] = macAddr[idx];

   // Send the configuration data to the Ethernet chip
   for (idx = 1; idx < W5X00_REGISTER_BLOCK_SIZE; ++idx)
#if ETHERNET_CHIP == WIZNET_W5500
      w5x00WriteReg(idx, 0x04, registerBuffer[idx]);
#else
      w5x00WriteReg(idx, 0, registerBuffer[idx]);
#endif

#if ETHERNET_CHIP == WIZNET_W5500
   uint8_t controlByte;

   // Assign 2KB RX and TX memory per socket
   for (idx = 0; idx < 8; ++idx)
   {
      controlByte = (0x0C + (idx << 5));
      w5x00WriteReg(0x1E, controlByte, 2);   //0x1E - Sn_RXBUF_SIZE
      w5x00WriteReg(0x1F, controlByte, 2);   //0x1F - Sn_TXBUF_SIZE
   }
#endif

   return true;
}

void w5x00End (void)
{
   W5X00_END_CS;

   w5x00ResetEndTime = 0xFFFFFFFFFFFFFFFF;
}

uint8_t w5x00ReadReg (uint16_t address, uint8_t cb)
{
   uint8_t receivedByte;

   W5X00_ASSERT_CS;

#if ETHERNET_CHIP == WIZNET_W5100
   spiTransfer(W5X00_READ);
#endif

   spiTransfer16(address);

#if ETHERNET_CHIP == WIZNET_W5500
   spiTransfer(cb);  //Socket 3 BSB Write 0x6D Selects Socket 3 Register, write mode, 1 byte data length
#endif

   receivedByte = spiTransfer(0U);

   W5X00_DEASSERT_CS;

   return receivedByte;
}

uint16_t w5x00ReadWord (uint16_t address, uint8_t cb)
{
   return ((uint16_t)(w5x00ReadReg(address, cb)) << 8) | (uint16_t)(w5x00ReadReg(address + 1, cb));
}

void w5x00WriteReg (uint16_t address, uint8_t cb, uint8_t value)
{
   W5X00_ASSERT_CS;

#if ETHERNET_CHIP == WIZNET_W5100
   spiTransfer(W5X00_WRITE);
#endif

   spiTransfer16(address);

#if ETHERNET_CHIP == WIZNET_W5500
   spiTransfer(cb);  //Socket 3 BSB Write 0x6D Selects Socket 3 Register, write mode, 1 byte data length
#endif

   spiTransfer(value);

   W5X00_DEASSERT_CS;
}

void w5x00WriteWord (uint16_t address, uint8_t cb, uint16_t value)
{
   w5x00WriteReg(address, cb, (uint8_t)(value >> 8));
   w5x00WriteReg(++address, cb, (uint8_t)(value & 0xff));
}
