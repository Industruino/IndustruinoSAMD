/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (C) 2017 Industruino <connect@industruino.com>  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "variant.h"

/*
 * Pins descriptions
 */
const PinDescription g_APinDescription[]=
{
   // 0..1 - Digital pins, SERCOM/UART (Serial)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 0          | D0               |  PA19  | RX (Serial)    | EIC/EXTINT[3]  PTC/X[7]  SERCOM1/PAD[3] *SERCOM3/PAD[3] *TC3/WO[1]  TCC0/WO[3]
  // | 1          | D1               |  PA18  | TX (Serial)    | EIC/EXTINT[2]  PTC/X[6]  SERCOM1/PAD[2] *SERCOM3/PAD[2] *TC3/WO[0]  TCC0/WO[2]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 19, PIO_SERCOM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH1, TC3_CH1, EXTERNAL_INT_3 }, // RX: SERCOM3/PAD[3]
  { PORTA, 18, PIO_SERCOM_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH0, TC3_CH0, EXTERNAL_INT_2 }, // TX: SERCOM3/PAD[2]

  // 2..3 - Digital pins, I2C pins (SDA/SCL)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 2          | D2               |  PA16  | SDA (Wire)      | EIC/EXTINT[0] *SERCOM1/PAD[0] SERCOM3/PAD[0] *TCC2/WO[0] TCC0/WO[6]
  // | 3          | D3               |  PA17  | SCL (Wire)      | EIC/EXTINT[1] *SERCOM1/PAD[1] SERCOM3/PAD[1] *TCC2/WO[1] TCC0/WO[7]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 16, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH0, TCC2_CH0, EXTERNAL_INT_0 }, // SDA: SERCOM1/PAD[0]
  { PORTA, 17, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH1, TCC2_CH1, EXTERNAL_INT_1 }, // SCL: SERCOM1/PAD[1]

  // 4..13 - Digital and analog pins
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 4          | D4               |  PB08  |                 | EIC/EXTINT[8]  *ADC/AIN[2]            PTC/Y[14] SERCOM4/PAD[0]                 *TC4/WO[0]
  // | 5          | D5               |  PA10  | TX (Serial1)    | EIC/EXTINT[10] *ADC/AIN[18]           PTC/X[2] *SERCOM0/PAD[2]                  TCC0/WO[2] *TCC1/WO[0]
  // | 6          | D6               |  PB09  |                 | EIC/EXTINT[9]  *ADC/AIN[3]            PTC/Y[15] SERCOM4/PAD[1]                 *TC4/WO[1]
  // | 7          | D7               |  PA04  |                 | EIC/EXTINT[4]  *ADC/AIN[4]  AC/AIN[0] PTC/Y[2]  SERCOM0/PAD[0]                 *TCC0/WO[0]
  // | 8          | D8               |  PA05  |                 | EIC/EXTINT[5]  *ADC/AIN[5]  AC/AIN[1] PTC/Y[5]  SERCOM0/PAD[1]                 *TCC0/WO[1]
  // | 9          | D9               |  PA06  |                 | EIC/EXTINT[6]  *ADC/AIN[6]  AC/AIN[2] PTC/Y[4]  SERCOM0/PAD[2]                  TCC1/WO[0]
  // | 10         | D10              |  PA07  | RX (Serial1)    | EIC/EXTINT[7]  *ADC/AIN[7]  AC/AIN[3] PTC/Y[5] *SERCOM0/PAD[3]                  TCC1/WO[1]
  // | 11         | D11              |  PA08  |                 | EIC/NMI        *ADC/AIN[16]           PTC/X[0]  SERCOM0/PAD[0]  SERCOM2/PAD[0]  TCC0/WO[0] *TCC1/WO[2]
  // | 12         | D12              |  PA09  |                 | EIC/EXTINT[9]  *ADC/AIN[17]           PTC/X[1]  SERCOM0/PAD[1]  SERCOM2/PAD[1]  TCC0/WO[1] *TCC1/WO[3]
  // | 13         | D13              |  PA11  |                 | EIC/EXTINT[11] *ADC/AIN[19]           PTC/X[3]  SERCOM0/PAD[3]  SERCOM2/PAD[3] *TCC0/WO[3]  TCC1/WO[1]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTB,  8, PIO_ANALOG, (PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel2, PWM4_CH0, TC4_CH0, EXTERNAL_INT_8 }, // ADC/AIN[2]
  { PORTA, 10, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel18, PWM1_CH0, TCC1_CH0, EXTERNAL_INT_10 }, // TX: SERCOM0/PAD[2]
  { PORTB,  9, PIO_ANALOG, (PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel3, PWM4_CH1, TC4_CH1, EXTERNAL_INT_9 }, // ADC/AIN[3]
  { PORTA,  4, PIO_ANALOG, (PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel4, PWM0_CH0, TCC0_CH0, EXTERNAL_INT_4 }, // ADC/AIN[4]
  { PORTA,  5, PIO_ANALOG, (PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel5, PWM0_CH1, TCC0_CH1, EXTERNAL_INT_5 }, // ADC/AIN[5]
  { PORTA,  6, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel6, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_6 },
  { PORTA,  7, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, ADC_Channel7, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_7 }, // RX: SERCOM0/PAD[3]
  { PORTA,  8, PIO_ANALOG, (PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), ADC_Channel16, PWM1_CH2, TCC1_CH2, EXTERNAL_INT_NMI }, // TCC1/WO[2]
  { PORTA,  9, PIO_ANALOG, (PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), ADC_Channel17, PWM1_CH3, TCC1_CH3, EXTERNAL_INT_9 }, // TCC1/WO[3]
  { PORTA, 11, PIO_ANALOG, (PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), ADC_Channel19, PWM0_CH3, TCC0_CH3, EXTERNAL_INT_11 }, // TCC0/WO[3]

  // 14..17 - SPI pins (Expansion port: MISO, SCK, MOSI)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 14         | D14              |  PB22  | MISO (SPI)      | EIC/EXTINT[6]                            *SERCOM5/PAD[2]
  // | 15         | D15              |  PA23  | SCK (SPI)       | EIC/EXTINT[7]  PTC/X[11]  SERCOM3/PAD[1] *SERCOM5/PAD[1]  TC4/WO[1] *TCC0/WO[5]
  // | 16         | D16              |  PB23  | MOSI (SPI)      | EIC/EXTINT[7]                            *SERCOM5/PAD[3]
  // | 17         | D17              |  PA22  |                 | EIC/EXTINT[6]  PTC/X[10]  SERCOM3/PAD[0]  SERCOM5/PAD[0]  TC4/WO[0] *TCC0/WO[4]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTB, 22, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_6 }, // MISO: SERCOM5/PAD[2]
  { PORTA, 23, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_7 }, // SCK: SERCOM5/PAD[1]
  { PORTB, 23, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_7 }, // MOSI: SERCOM5/PAD[3]
  { PORTA, 22, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // SS

  // 18 - Digital and analog pin
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 18         | D18              |  PA02  |                 | EIC/EXTINT[2] *ADC/AIN[0]  DAC/VOUT  PTC/Y[0]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 2, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // ADC/AIN[0]

  // 19..21 - SPI pins (LCD: CS, MOSI, SCK)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 19         |                  |  PB10  |                 | EIC/EXTINT[10]                 SERCOM4/PAD[2] TC5/WO[0]  TCC0/WO[4]
  // | 20         |                  |  PA12  | MOSI (SPI1)     | EIC/EXTINT[12] SERCOM2/PAD[0] *SERCOM4/PAD[0] TCC2/WO[0] TCC0/WO[6]
  // | 21         |                  |  PB11  | SCK (SPI1)      | EIC/EXTINT[11]                *SERCOM4/PAD[3] TC5/WO[1]  TCC0/WO[5]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTB, 10, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_10 }, // CS
  { PORTA, 12, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_12 }, // MOSI: SERCOM4/PAD[0]
  { PORTB, 11, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11 }, // SCK: SERCOM4/PAD[3]

  // 22 - LCD CD pin
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 22         |                  |  PB03  |                 | EIC/EXTINT[3]  AIN[11]  PTC/Y[9]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTB, 3, PIO_OUTPUT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // CD

  // 23..25 - BUTTONS
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 23         |                  |  PA15  |                 | EIC/EXTINT[15]  SERCOM2/PAD[3]  SERCOM4/PAD[3]  TC3/WO[1]   TCC0/WO[5]
  // | 24         |                  |  PA14  |                 | EIC/EXTINT[14]  SERCOM2/PAD[2]  SERCOM4/PAD[2]  TC3/WO[0]   TCC0/WO[4]
  // | 25         |                  |  PA13  |                 | EIC/EXTINT[13]  SERCOM2/PAD[1]  SERCOM4/PAD[1]  TCC2/WO[1]  TCC0/WO[7]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 15, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_15 }, // BTN-1
  { PORTA, 14, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_14 }, // BTN-2
  { PORTA, 13, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_13 }, // BTN-3

  // 26 - LCD backlight
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 26         |                  |  PA20  | LCD backlight   | EIC/EXTINT[4]  PTC/X[8]  SERCOM5/PAD[2]  SERCOM3/PAD[2] *TCC0/WO[6]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 20, PIO_PWM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH6, TCC0_CH6, EXTERNAL_INT_4 }, // TCC0/WO[6]

  // 27 - RTC alarm
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 27         |                  |  PB02  |                 | EIC/EXTINT[2] *ADC/AIN[10]  PTC/Y[8]  SERCOM5/PAD[0]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTB, 2, PIO_ANALOG, 0, ADC_Channel10, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // ADC/AIN[10]

  // 28 - (AREF)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 28         | D28              |  PA03  | AREF            | EIC/EXTINT[3] *[ADC|DAC]/VREFA  ADC/AIN[1]  PTC/Y[1]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 3, PIO_ANALOG, PIN_ATTR_ANALOG, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_3 }, // DAC/VREFP

  // 29 - Alternate use of D18/A18 (DAC output)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 29         | D18              |  PA02  | (DAC0)          | EIC/EXTINT[2]  ADC/AIN[0] *DAC/VOUT  PTC/Y[0]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 2, PIO_ANALOG, PIN_ATTR_ANALOG, DAC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // DAC/VOUT

  // 30..32 - USB
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 30         |                  |  PA28  | USB_HOST_ENABLE | EIC/EXTINT[8]
  // | 31         |                  |  PA24  | USB_NEGATIVE    | EIC/EXTINT[12]  SERCOM3/PAD[2]  SERCOM5/PAD[2]  TC5/WO[0]  TCC1/WO[2] *USB/DM
  // | 32         |                  |  PA25  | USB_POSITIVE    | EIC/EXTINT[13]  SERCOM3/PAD[3]  SERCOM5/PAD[3]  TC5/WO[1]  TCC1/WO[3] *USB/DP
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTA, 28, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB Host enable
  { PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DM
  { PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DP

  // 33 - SPI pin (LCD: MISO)
  // WARNING: this pin is connected to an external pad on the SAMD21J only.
  //          This definition for the SAMD21G is a workaround to define a pin for SERCOM4/PAD[2]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | Pin number |  D21G Board pin  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  // | 33         |                  |  PB14  | MISO (SPI1)     | EIC/EXTINT[14]  PTC/X[14]  SERCOM4/PAD[2]  TC5/WO[0]
  // +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
  { PORTB, 14, PIO_SERCOM_ALT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_14 }, // MISO: SERCOM4/PAD[2]
} ;

const void* g_apTCInstances[TCC_INST_NUM+TC_INST_NUM]={ TCC0, TCC1, TCC2, TC3, TC4, TC5 } ;

// Multi-serial objects instantiation
SERCOM sercom0( SERCOM0 ) ;
SERCOM sercom1( SERCOM1 ) ;
SERCOM sercom2( SERCOM2 ) ;
SERCOM sercom3( SERCOM3 ) ;
SERCOM sercom4( SERCOM4 ) ;
SERCOM sercom5( SERCOM5 ) ;

Uart Serial1( &sercom0, PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX ) ;
Uart Serial( &sercom3, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX ) ;
void SERCOM0_Handler()
{
  Serial1.IrqHandler();
}

void SERCOM3_Handler()
{
  Serial.IrqHandler();
}
