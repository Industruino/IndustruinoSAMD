/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.
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

#ifndef _BOARD_DEFINITIONS_H_
#define _BOARD_DEFINITIONS_H_

#define BOOTLOADER_MAX_RUN_TIME (7000ULL*48ULL)   // 7 seconds in system ticks

/*
 * USB device definitions
 */
#define STRING_MANUFACTURER "ES Gear Ltd."
#define STRING_PRODUCT      "Industruino D21G"

#define USB_VID_HIGH   0x2E
#define USB_VID_LOW    0x78
#define USB_PID_HIGH   0x00
#define USB_PID_LOW    0x4D

/*
 * REBOOT_STATUS_ADDRESS must point to a free SRAM cell that must not
 * be touched from the loaded application.
 */
#define REBOOT_STATUS_ADDRESS  (0x20007FFCul)
#define REBOOT_STATUS_VALUE    (*((volatile uint32_t *) REBOOT_STATUS_ADDRESS))

/*
 * REBOOT_STATUS_UNDEFINED
 *    No specific action defined for the next warm reset.  This status is
 *    assigned right after a cold reset or when no behaviour after the next warm
 *    reset has been defined yet
 *
 * REBOOT_STATUS_DOUBLE_TAP_MAGIC
 *    Quick two times tapping of the reset button detected: run the bootloader
 *    until a successful programming of the FLASH memory has been completed
 *
 * REBOOT_STATUS_START_APP
 *    Start the application right after the warm reset
 */
#define REBOOT_STATUS_UNDEFINED        (0)
#define REBOOT_STATUS_DOUBLE_TAP_MAGIC (0x07738135)
#define REBOOT_STATUS_START_APP        (0x63313669)

/*
 * If BOOT_LOAD_PIN is defined the bootloader is started if the selected
 * pin is tied LOW.
 */
//#define BOOT_LOAD_PIN                     PIN_PA21 // Pin 7
//#define BOOT_LOAD_PIN                     PIN_PA15 // Pin 5

#define BOOT_USART_MODULE                 SERCOM0
#define BOOT_USART_BUS_CLOCK_INDEX        PM_APBCMASK_SERCOM0
#define BOOT_USART_PER_CLOCK_INDEX        GCLK_CLKCTRL_ID_SERCOM0_CORE_Val
#define BOOT_USART_PAD_SETTINGS           UART_RX_PAD3_TX_PAD2
#define BOOT_USART_PAD3                   PINMUX_PA11C_SERCOM0_PAD3
#define BOOT_USART_PAD2                   PINMUX_PA10C_SERCOM0_PAD2
#define BOOT_USART_PAD1                   PINMUX_UNUSED
#define BOOT_USART_PAD0                   PINMUX_UNUSED


/* Master clock frequency */
#define CPU_FREQUENCY                     (48000000ul)
#define VARIANT_MCK                       CPU_FREQUENCY

/* Frequency of the board main oscillator */
#define VARIANT_MAINOSC                   (32768ul)

/* Calibration values for DFLL48 pll */
#define NVM_SW_CALIB_DFLL48M_COARSE_VAL   (58)
#define NVM_SW_CALIB_DFLL48M_FINE_VAL     (64)

/*
 * LEDs definitions
 */
#define BOARD_LED_PORT                    (0)
#define BOARD_LED_PIN                     (20)

#define BOARD_LEDRX_PORT                  (0)
#define BOARD_LEDRX_PIN                   (20)

#define BOARD_LEDTX_PORT                  (0)
#define BOARD_LEDTX_PIN                   (20)

/*
 * SPI definitions and configuration
 */

#define SPI_SERCOM                        SERCOM5
#define SPI_PM_APBCMASK                   PM_APBCMASK_SERCOM5
#define SPI_PER_CLOCK_ID                  GCLK_CLKCTRL_ID_SERCOM5_CORE_Val
#define SPI_NVIC_ID                       SERCOM5_IRQn
#define SPI_PERIPHERAL_MUX                (3U)           // (PIO_SERCOM_ALT, see 'WVariant.h', EPioType)
#define SPI_MISO_PGROUP                   (1U)           // Port B (see 'WVariant.h')
#define SPI_MISO_PIN                      (22U)          // PB22
#define SPI_MOSI_PGROUP                   (1U)           // Port B (see 'WVariant.h')
#define SPI_MOSI_PIN                      (23U)          // PB23
#define SPI_SCK_PGROUP                    (0U)           // Port A (see 'WVariant.h')
#define SPI_SCK_PIN                       (23U)          // PA23
#define SPI_TX_PAD                        (2U)           // (SPI_PAD_3_SCK_1, see 'SERCOM.h', SercomSpiTXPad)
#define SPI_RX_PAD                        (2U)           // (SERCOM_RX_PAD_2, see 'SERCOM.h', SercomRXPad)

#define SPI_BITRATE                       (4000000UL)     // 4 MHz
#define SPI_MAX_BITRATE                   (12000000UL)    // 12 MHz max, see 'SPI.h', SPI_MIN_CLOCK_DIVIDER
#define SPI_MSB_FIRST                     (0U)            // MSB_FIRST, see 'SERCOM.h' (SercomDataOrder)
#define SPI_LSB_FIRST                     (1U)            // LSB_FIRST, see 'SERCOM.h' (SercomDataOrder)
#define SPI_DATA_ORDER                    SPI_MSB_FIRST
#define SPI_CLOCK_MODE                    (0U)            // Mode 0 (SERCOM_SPI_MODE_0, see 'SERCOM.h', SercomSpiClockMode)
#define SPI_CHAR_SIZE                     (0UL)           // 8 bits (SPI_CHAR_SIZE_8_BITS, see 'SERCOM.h', SercomSpiCharSize)

/*
 * Ethernet module/chip definitions
 */

#define ETHERNET_MODULE_ENABLE_PORT       (1U)    // Port B
#define ETHERNET_MODULE_ENABLE_PIN        (11U)   // PB11

// Wiznet Ethernet chip RESET and CS pin
#define W5X00_USE_HARDWARE_RESET          true
#define W5X00_RESET_PORT                  (0U)              // Port A
#define W5X00_RESET_PIN                   (10U)             // PA10
#define W5X00_RESET_LEVEL_HIGH            true              // Set this to 'false' if an Industruino Ethernet module revision < 5.0 is used
#define W5X00_POST_RESET_DELAY            (2000ULL*48ULL)   // 2 seconds in system ticks
#define W5X00_CS_PORT                     (0U)              // Port A
#define W5X00_CS_PIN                      (7U)              // PA07

// FRAM chip CS pin
#define FRAM_CS_PORT                      (1U)   // Port B
#define FRAM_CS_PIN                       (9U)   // PB09

// MB85RS64A - 64 K (8 K x 8) bit SPI FRAM
#define FRAM_SIZE                         ((uint16_t)(0x2000))

// SD card CS pin
#define SDCARD_CS_PORT                    (1U)   // Port B
#define SDCARD_CS_PIN                     (8U)   // PB08

/*
 * Networking configuration
 */

#define NETCFG_DEFAULT_MAC_ADDR           0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#define NETCFG_DEFAULT_IP_ADDR            192, 168, 0, 200
#define NETCFG_DEFAULT_NETWORK_MASK       255, 255, 255, 0
#define NETCFG_DEFAULT_GATEWAY_ADDR       192, 168, 0, 1
#define NETCFG_DEFAULT_TFTP_DATA_PORT     46969
#define NETCFG_DEFAULT_PASSWORD           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '   // 8 chars long (if shorter fill with spaces)

#endif // _BOARD_DEFINITIONS_H_
