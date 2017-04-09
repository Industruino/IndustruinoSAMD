//  Minimal SPI driver
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
//  (code adapted from Arduino SAMD core)

#include <stdbool.h>
#include <sam.h>
#include "spi.h"
#include "board_definitions.h"

#define SERCOM_FREQ_REF (48000000)   // See 'SERCOM.h'

static bool spiInitialized = false;

void setPeripheralPinMux (uint8_t portGroup, uint32_t pinNumber, int peripheral)
{
   uint32_t tmp;

   if (pinNumber & 1)
   {
      // It's an odd pin number
      tmp = (PORT->Group[portGroup].PMUX[pinNumber >> 1].reg) & PORT_PMUX_PMUXE(0xF);
      PORT->Group[portGroup].PMUX[pinNumber >> 1].reg = tmp | PORT_PMUX_PMUXO(peripheral);
      PORT->Group[portGroup].PINCFG[pinNumber].reg |= PORT_PINCFG_PMUXEN;
   }
   else
   {
      // It's an even pin number
      tmp = (PORT->Group[portGroup].PMUX[pinNumber >> 1].reg) & PORT_PMUX_PMUXO(0xF);
      PORT->Group[portGroup].PMUX[pinNumber >> 1].reg = tmp | PORT_PMUX_PMUXE(peripheral);
      PORT->Group[portGroup].PINCFG[pinNumber].reg |= PORT_PINCFG_PMUXEN;
   }
}

static void spiReset (void)
{
   SPI_SERCOM->SPI.CTRLA.bit.SWRST = 1;
   while (SPI_SERCOM->SPI.CTRLA.bit.SWRST || SPI_SERCOM->SPI.SYNCBUSY.bit.SWRST);

   spiInitialized = false;
}

void spiInit (uint32_t bitrate)
{
   int cpha, cpol;

   if (spiInitialized)
      return;

   if (bitrate > SPI_MAX_BITRATE)
      bitrate = SPI_MAX_BITRATE;

   setPeripheralPinMux(SPI_MISO_PGROUP, SPI_MISO_PIN, SPI_PERIPHERAL_MUX);
   setPeripheralPinMux(SPI_SCK_PGROUP, SPI_SCK_PIN, SPI_PERIPHERAL_MUX);
   setPeripheralPinMux(SPI_MOSI_PGROUP, SPI_MOSI_PIN, SPI_PERIPHERAL_MUX);

   // Disable SPI
   SPI_SERCOM->SPI.CTRLA.bit.ENABLE = 0;
   while (SPI_SERCOM->SPI.SYNCBUSY.bit.ENABLE);

   spiReset();

   // Setting clock
   PM->APBCMASK.reg |= SPI_PM_APBCMASK;
   GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SPI_PER_CLOCK_ID) |
                       GCLK_CLKCTRL_GEN_GCLK0 |
                       GCLK_CLKCTRL_CLKEN;
   while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

   // Peripheral configuration
   SPI_SERCOM->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
                               SERCOM_SPI_CTRLA_DOPO(SPI_TX_PAD) |
                               SERCOM_SPI_CTRLA_DIPO(SPI_RX_PAD) |
                               SPI_DATA_ORDER << SERCOM_SPI_CTRLA_DORD_Pos;
   SPI_SERCOM->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_CHSIZE(SPI_CHAR_SIZE) |
                               SERCOM_SPI_CTRLB_RXEN;

   cpha = (SPI_CLOCK_MODE & (0x1ul)) == 0 ? 0 : 1;
   cpol = (SPI_CLOCK_MODE & (0x2ul)) == 0 ? 0 : 1;

   SPI_SERCOM->SPI.CTRLA.reg |= (cpha << SERCOM_SPI_CTRLA_CPHA_Pos) |
                                (cpol << SERCOM_SPI_CTRLA_CPOL_Pos);

   SPI_SERCOM->SPI.BAUD.reg = SERCOM_FREQ_REF / (2 * bitrate) - 1;

   // Enable SPI
   SPI_SERCOM->SPI.CTRLA.bit.ENABLE = 1;
   while (SPI_SERCOM->SPI.SYNCBUSY.bit.ENABLE);

   // See 'Atmel-42181N-SAM-D21_Datasheet_Complete-10/2016', page 519, Bit 17 - RXEN
   while (SPI_SERCOM->SPI.SYNCBUSY.bit.CTRLB);
   while (!SPI_SERCOM->SPI.CTRLB.bit.RXEN);

   spiInitialized = true;
}

void spiEnd (void)
{
   spiReset();
}

uint8_t spiTransfer (uint8_t data)
{
   SPI_SERCOM->SPI.DATA.bit.DATA = data;

   while (SPI_SERCOM->SPI.INTFLAG.bit.RXC == 0);

   return SPI_SERCOM->SPI.DATA.bit.DATA;
}

void spiTransferBytes (uint8_t *data, uint16_t size)
{
   while (size--)
   {
      SPI_SERCOM->SPI.DATA.bit.DATA = *(data++);

      while (SPI_SERCOM->SPI.INTFLAG.bit.RXC == 0);
   }
}

uint16_t spiTransfer16 (uint16_t data)
{
   union
   {
      uint16_t val;
      struct
      {
         uint8_t lsb;
         uint8_t msb;
      };
   } t;

   t.val = data;

   if (SPI_DATA_ORDER == SPI_LSB_FIRST)
   {
      t.lsb = spiTransfer(t.lsb);
      t.msb = spiTransfer(t.msb);
   }
   else
   {
      t.msb = spiTransfer(t.msb);
      t.lsb = spiTransfer(t.lsb);
   }

   return t.val;
}
