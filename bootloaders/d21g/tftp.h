//  Partial TFTP implementation
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
//     Project: ethboot
//     Function: tftp implementation and flasher
//     Version: 0.2 tftp / flashing functional

#ifndef __TFTP_H__
#define __TFTP_H__

#include <stdbool.h>
#include <sam.h>

#define TFTP_RXRQ_NONE (0U)
#define TFTP_RXRQ_RRQ  (1U)
#define TFTP_RXRQ_WRQ  (2U)

#define TFTP_STATUS_ERROR_CRC           ((int8_t) -6)
#define TFTP_STATUS_ERROR_PASSWORD      ((int8_t) -5)
#define TFTP_STATUS_ERROR_BAD_IMAGE     ((int8_t) -4)
#define TFTP_STATUS_ERROR_FULL          ((int8_t) -3)
#define TFTP_STATUS_ERROR_NOT_SUPPORTED ((int8_t) -2)
#define TFTP_STATUS_ERROR_UNKNOWN       ((int8_t) -1)
#define TFTP_STATUS_NO_TRAFFIC          ((int8_t) 0)
#define TFTP_STATUS_SEND_ACK            ((int8_t) 1)
#define TFTP_STATUS_RX_COMPLETED        ((int8_t) 2)

#define TFTP_IS_ERROR_STATUS(x) (x < TFTP_STATUS_NO_TRAFFIC)
#define TFTP_IS_RX_COMPLETED(x) (x == TFTP_STATUS_RX_COMPLETED)


void tftpInit (void);
bool tftpIsReady (void);
bool tftpConfigure (void);
void tftpEnd (void);
int8_t tftpRun (void);
bool tftpSendResponse (int8_t status);

uint8_t tftpReceivedRequest (void);
bool tftpGetReceivedBytes (uint8_t *bytesBuffer, uint16_t bytesNumber);
bool tftpGetReceivedWords (uint8_t *bytesBuffer, uint16_t wordsNumber);
uint16_t tftpReceivedBytesCount (void);


#endif   // __TFTP_H__
