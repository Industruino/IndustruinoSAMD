//  Basic read function for the MB85RS64A SPI FRAM chip
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

#include <sam.h>
#include "board_definitions.h"
#include "fram.h"
#include "spi.h"

#define FRAM_INIT_CS     PORT->Group[FRAM_CS_PORT].DIRSET.reg=(1<<FRAM_CS_PIN)
#define FRAM_END_CS      PORT->Group[FRAM_CS_PORT].DIRCLR.reg=(1<<FRAM_CS_PIN)
#define FRAM_ASSERT_CS   PORT->Group[FRAM_CS_PORT].OUTCLR.reg=(1<<FRAM_CS_PIN)
#define FRAM_DEASSERT_CS PORT->Group[FRAM_CS_PORT].OUTSET.reg=(1<<FRAM_CS_PIN)

#define FRAM_CMD_READ  0x03


void framInit (void)
{
   FRAM_DEASSERT_CS;
   FRAM_INIT_CS;
}

void framEnd (void)
{
   FRAM_END_CS;
}

bool framRead (uint16_t addr, uint8_t *dataBuffer, uint16_t count)
{
   if (addr + count > FRAM_SIZE)
      return false;

   if (count == 0U)
      return true;

   FRAM_ASSERT_CS;

   spiTransfer(FRAM_CMD_READ);
   spiTransfer16(addr);

   for (uint16_t i=0; i < count; ++i)
      dataBuffer[i] = spiTransfer(0x00);

   FRAM_DEASSERT_CS;

   return true;
}
