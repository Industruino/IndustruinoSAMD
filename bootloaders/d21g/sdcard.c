//  Basic init function for the SD card
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
#include "sdcard.h"
#include "spi.h"
#include "utils.h"

#define SDCARD_INIT_CS     PORT->Group[SDCARD_CS_PORT].DIRSET.reg=(1<<SDCARD_CS_PIN)
#define SDCARD_END_CS      PORT->Group[SDCARD_CS_PORT].DIRCLR.reg=(1<<SDCARD_CS_PIN)
#define SDCARD_ASSERT_CS   PORT->Group[SDCARD_CS_PORT].OUTCLR.reg=(1<<SDCARD_CS_PIN)
#define SDCARD_DEASSERT_CS PORT->Group[SDCARD_CS_PORT].OUTSET.reg=(1<<SDCARD_CS_PIN)

#define SDCARD_INIT_TIMEOUT             (2000ULL*48ULL)   // 2 seconds in system ticks
#define SDCARD_BUSY_CHECK_RETRY_TIMEOUT (300ULL*48ULL)    // 300 millisconds in system ticks

#define SDCARD_INIT_SPI_BITRATE (250000)

#define SDCARD_RSP_IDLE_STATE ((uint8_t) 0x01)
#define SDCARD_CMD0_SIZE      ((uint16_t) 6)
uint8_t sdCardCmd0Bytes[SDCARD_CMD0_SIZE] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x95 };


void sdcardInit (void)
{
   SDCARD_DEASSERT_CS;
   SDCARD_INIT_CS;
}

uint8_t sdCardIsBusy (uint64_t timeoutTicks)
{
   uint32_t timeoutTime = millis() + timeoutTicks;

   do
   {
      if (spiTransfer(0xFF) == 0xFF)
         return false;
   }
   while (millis() < timeoutTime);

   return true;
}

bool sdCardSetSpiMode (void)
{
   spiEnd();
   spiInit(SDCARD_INIT_SPI_BITRATE);

   uint32_t initializationTimeoutTime = millis() + SDCARD_INIT_TIMEOUT;
   bool returnValue = false;

   for (uint8_t i = 0; i < 10; i++)
      spiTransfer(0xFF);

   SDCARD_ASSERT_CS;

   while (millis() <= initializationTimeoutTime)
   {
      if (sdCardIsBusy(SDCARD_BUSY_CHECK_RETRY_TIMEOUT))
         continue;

      spiTransferBytes(sdCardCmd0Bytes, SDCARD_CMD0_SIZE);

      uint8_t response;

      for (uint8_t i = 0; ((response = spiTransfer(0xFF)) & 0x80) && (i < 0xFF); ++i);

      if (response == SDCARD_RSP_IDLE_STATE)
      {
         returnValue = true;
         break;
      }
   }

   SDCARD_DEASSERT_CS;

   spiEnd();
   spiInit(SPI_BITRATE);

   return returnValue;
}