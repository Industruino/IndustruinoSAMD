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

#ifndef __NETWORKING_H__
#define __NETWORKING_H__

#include <stdbool.h>
#include <sam.h>


bool netInit (void);
void netEnd (void);

void netInitUdpSocket3 (uint16_t port);

void netOpenSocket3 (void);
void netCloseSocket3 (void);

uint16_t netReceivedDataSizeSocket3 (void);
void netSync (uint32_t msDelay);

void netSendPacketSocket3 (uint8_t *txPtr, uint16_t packetSize);
void netReceivePacketSocket3 (uint8_t* buffer, uint16_t packetSize);


#endif   // __NETWORKING_H__
