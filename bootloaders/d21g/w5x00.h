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

#ifndef __W5X00_H__
#define __W5X00_H__

#include <stdbool.h>
#include <sam.h>
#include "board_definitions.h"

// Supported Wiznet Ethernet chips
#define WIZNET_W5100                      (1)
#define WIZNET_W5500                      (5)

#if ETHERNET_CHIP == WIZNET_W5100
#include "w5100.h"
#elif ETHERNET_CHIP == WIZNET_W5500
#include "w5500.h"
#else
#error "Ethernet chip not supported"
#endif


bool w5x00Init (void);
void w5x00Config (uint8_t macAddr[], uint8_t ipAddr[], uint8_t netMask[], uint8_t gwAddr[]);
void w5x00End (void);

uint8_t w5x00ReadReg (uint16_t address, uint8_t cb);
uint16_t w5x00ReadWord (uint16_t address, uint8_t cb);

void w5x00WriteReg (uint16_t address, uint8_t cb, uint8_t value);
void w5x00WriteWord (uint16_t address, uint8_t cb, uint16_t value);


#endif   // __W5X00_H__
