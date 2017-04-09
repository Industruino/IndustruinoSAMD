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

#define SDCARD_INIT_CS     PORT->Group[SDCARD_CS_PORT].DIRSET.reg=(1<<SDCARD_CS_PIN)
#define SDCARD_END_CS      PORT->Group[SDCARD_CS_PORT].DIRCLR.reg=(1<<SDCARD_CS_PIN)
#define SDCARD_ASSERT_CS   PORT->Group[SDCARD_CS_PORT].OUTCLR.reg=(1<<SDCARD_CS_PIN)
#define SDCARD_DEASSERT_CS PORT->Group[SDCARD_CS_PORT].OUTSET.reg=(1<<SDCARD_CS_PIN)


void sdcardInit (void)
{
   SDCARD_DEASSERT_CS;
   SDCARD_INIT_CS;
}
