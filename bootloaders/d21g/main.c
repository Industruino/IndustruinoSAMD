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

#include <stdio.h>
#include <sam.h>
#include "sam_ba_monitor.h"
#include "sam_ba_serial.h"
#include "board_definitions.h"
#include "board_driver_led.h"
#include "sam_ba_usb.h"
#include "sam_ba_cdc.h"
#include "tftp.h"
#include "utils.h"

extern void board_init(void);

static volatile bool main_b_cdc_enable = false;

static bool exitBootloaderAfterTimeout = true;
static bool usbCdcEnabled = false;

static bool ethernetModuleEnabled;

/**
 * \brief Check the application startup condition
 *
 */
static void check_start_application(void)
{
#if defined(REBOOT_STATUS_ADDRESS)
  if (PM->RCAUSE.bit.POR || PM->RCAUSE.bit.BOD12 || PM->RCAUSE.bit.BOD33)
     REBOOT_STATUS_VALUE = REBOOT_STATUS_UNDEFINED;
  else if (REBOOT_STATUS_VALUE == REBOOT_STATUS_START_APP)
    jumpToApplication();
#endif

//  LED_init();
//  LED_off();

  /*
   * Test sketch stack pointer @ &__sketch_vectors_ptr
   * Stay in SAM-BA if value @ (&__sketch_vectors_ptr) == 0xFFFFFFFF (Erased flash cell value)
   */
  if (__sketch_vectors_ptr == 0xFFFFFFFF)
  {
    /* Stay in bootloader */
    exitBootloaderAfterTimeout = false;
    usbCdcEnabled = true;
    return;
  }

  /*
   * Test vector table address of sketch @ &__sketch_vectors_ptr
   * Stay in SAM-BA if this function is not aligned enough, ie not valid
   */
  if ( ((uint32_t)(&__sketch_vectors_ptr) & ~SCB_VTOR_TBLOFF_Msk) != 0x00)
  {
    /* Stay in bootloader */
    exitBootloaderAfterTimeout = false;
    usbCdcEnabled = true;
    return;
  }

#if defined(REBOOT_STATUS_ADDRESS)
  if (PM->RCAUSE.bit.EXT)
  {
    if (REBOOT_STATUS_VALUE == REBOOT_STATUS_DOUBLE_TAP_MAGIC)
    {
      /* Second tap, stay in bootloader */
      REBOOT_STATUS_VALUE = REBOOT_STATUS_UNDEFINED;
      exitBootloaderAfterTimeout = false;
      usbCdcEnabled = true;
      return;
    }

    /* First tap */
    REBOOT_STATUS_VALUE = REBOOT_STATUS_DOUBLE_TAP_MAGIC;

    /* Wait 0.5sec to see if the user tap reset again.
     * The loop value is based on SAMD21 default 1MHz clock @ reset.
     */
    for (uint32_t i=0; i<125000; i++) /* 500ms */
      /* force compiler to not optimize this... */
      __asm__ __volatile__("");

    /* Timeout happened, continue boot... */
    REBOOT_STATUS_VALUE = REBOOT_STATUS_UNDEFINED;
  }
#endif

  if (!ethernetModuleEnabled)
     jumpToApplication();

  exitBootloaderAfterTimeout = true;
  usbCdcEnabled = false;

/*
#if defined(BOOT_LOAD_PIN)
  volatile PortGroup *boot_port = (volatile PortGroup *)(&(PORT->Group[BOOT_LOAD_PIN / 32]));
  volatile bool boot_en;

  // Enable the input mode in Boot GPIO Pin
  boot_port->DIRCLR.reg = BOOT_PIN_MASK;
  boot_port->PINCFG[BOOT_LOAD_PIN & 0x1F].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
  boot_port->OUTSET.reg = BOOT_PIN_MASK;
  // Read the BOOT_LOAD_PIN status
  boot_en = (boot_port->IN.reg) & BOOT_PIN_MASK;

  // Check the bootloader enable condition
  if (!boot_en)
  {
    // Stay in bootloader
    return;
  }
#endif
*/

//  LED_on();
}

#if DEBUG_ENABLE
#	define DEBUG_PIN_HIGH 	port_pin_set_output_level(BOOT_LED, 1)
#	define DEBUG_PIN_LOW 	port_pin_set_output_level(BOOT_LED, 0)
#else
#	define DEBUG_PIN_HIGH 	do{}while(0)
#	define DEBUG_PIN_LOW 	do{}while(0)
#endif

/**
 *  \brief SAMD21 SAM-BA Main loop.
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
  /* Read the configuration pin */
  PORT->Group[ETHERNET_MODULE_ENABLE_PORT].DIRCLR.reg = (1 << ETHERNET_MODULE_ENABLE_PIN);
  PORT->Group[ETHERNET_MODULE_ENABLE_PORT].PINCFG[ETHERNET_MODULE_ENABLE_PIN].reg = PORT_PINCFG_INEN;
  ethernetModuleEnabled = (PORT->Group[ETHERNET_MODULE_ENABLE_PORT].IN.reg & (1 << ETHERNET_MODULE_ENABLE_PIN) ? true : false);

#if SAM_BA_INTERFACE == SAM_BA_USBCDC_ONLY  ||  SAM_BA_INTERFACE == SAM_BA_BOTH_INTERFACES
  P_USB_CDC pCdc = NULL;
#endif
  DEBUG_PIN_HIGH;

  /* Jump in application if condition is satisfied */
  check_start_application();

  /* We have determined we should stay in the monitor. */
  /* System initialization */
  board_init();
  __enable_irq();

#if SAM_BA_INTERFACE == SAM_BA_UART_ONLY  ||  SAM_BA_INTERFACE == SAM_BA_BOTH_INTERFACES
  /* UART is enabled in all cases */
  serial_open();
#endif

#if SAM_BA_INTERFACE == SAM_BA_USBCDC_ONLY  ||  SAM_BA_INTERFACE == SAM_BA_BOTH_INTERFACES
  if(usbCdcEnabled) pCdc = usb_init();
#endif

#if SAM_BA_NET_INTERFACE == SAM_BA_NET_TFTP
  bool tftpInitializationNotStarted = true;
  bool tftpIsEnabled = false;
#endif

  DEBUG_PIN_LOW;

  /* Initialize LEDs */
  LED_init();
  LEDRX_init();
  LEDRX_off();
  LEDTX_init();
  LEDTX_off();

  /* Start the sys tick (1/48th of a millisecond) */
  SysTick_Config(1000);

  uint64_t bootloaderExitTime = millis() + BOOTLOADER_MAX_RUN_TIME;

  /* Wait for a complete enum on usb or a '#' char on serial line */
  while (1)
  {
#if SAM_BA_INTERFACE == SAM_BA_USBCDC_ONLY  ||  SAM_BA_INTERFACE == SAM_BA_BOTH_INTERFACES
    if (usbCdcEnabled && (pCdc->IsConfigured(pCdc) != 0))
    {
      main_b_cdc_enable = true;
    }

    /* Check if a USB enumeration has succeeded and if comm port has been opened */
    if (main_b_cdc_enable)
    {
#if SAM_BA_NET_INTERFACE == SAM_BA_NET_TFTP
      if (tftpIsEnabled) tftpEnd();
#endif
      sam_ba_monitor_init(SAM_BA_INTERFACE_USBCDC);

      /* SAM-BA on USB loop */
      sam_ba_monitor_run(exitBootloaderAfterTimeout);
    }
#endif

#if SAM_BA_INTERFACE == SAM_BA_UART_ONLY  ||  SAM_BA_INTERFACE == SAM_BA_BOTH_INTERFACES
    /* Check if a '#' has been received */
    if (!main_b_cdc_enable && serial_sharp_received())
    {
#if SAM_BA_NET_INTERFACE == SAM_BA_NET_TFTP
      if (tftpIsEnabled) tftpEnd();
#endif
      sam_ba_monitor_init(SAM_BA_INTERFACE_USART);

      /* SAM-BA on Serial loop */
      sam_ba_monitor_run(exitBootloaderAfterTimeout);
    }
#endif

#if SAM_BA_NET_INTERFACE == SAM_BA_NET_TFTP
    if (tftpInitializationNotStarted && ethernetModuleEnabled)
    {
        tftpInit();
        tftpInitializationNotStarted = false;
    }

    if (!tftpInitializationNotStarted && !tftpIsEnabled && ethernetModuleEnabled)
    {
        if (tftpIsReady())
           tftpIsEnabled = tftpConfigure();
    }

    if (tftpIsEnabled)
    {
      int8_t tftpStatus = tftpRun();

      /* Check if a TFTP write request has been received */
      if (!main_b_cdc_enable && (tftpReceivedRequest() == TFTP_RXRQ_WRQ))
      {
        tftpSendResponse(tftpStatus);

        sam_ba_monitor_init(SAM_BA_NET_TFTP);

        /* SAM-BA on TFTP loop */
        sam_ba_monitor_run(exitBootloaderAfterTimeout);
      }
      else
      {
        tftpSendResponse(tftpStatus);

        if (TFTP_IS_ERROR_STATUS(tftpStatus))
        {
          tftpEnd();
          NVIC_SystemReset();
        }
      }
    }
#endif

    if (exitBootloaderAfterTimeout && (millis() > bootloaderExitTime))
      startApplication();
  }
}

void SysTick_Handler(void)
{
  ++tickCount;

  LED_pulse();

  sam_ba_monitor_sys_tick();
}
