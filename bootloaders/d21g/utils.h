//  Utility functions
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

#ifndef __DELAY_H__
#define __DELAY_H__

#include <sam.h>


// Exported value from linker script
extern uint32_t __sketch_vectors_ptr;

extern volatile uint64_t tickCount;

uint64_t millis (void);
void delay (uint32_t delayInMilliseconds);

void startApplication (void);
void jumpToApplication (void);


#endif   // __DELAY_H__
