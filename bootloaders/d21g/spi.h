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

#ifndef __SPI_H__
#define __SPI_H__


void spiInit (uint32_t bitrate);
void spiEnd (void);

uint8_t spiTransfer (uint8_t data);
void spiTransferBytes (uint8_t *data, uint16_t size);
uint16_t spiTransfer16 (uint16_t data);


#endif   // __SPI_H__
