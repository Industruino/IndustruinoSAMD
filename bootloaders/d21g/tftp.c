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

#include <string.h>
#include <stddef.h>
#include "tftp.h"
#include "netcfg.h"
#include "networking.h"

#define TFTP_PORT ((uint16_t) 69)

#define TFTP_OPCODE_UKN   (0)
// TFTP Opcode values from RFC 1350
#define TFTP_OPCODE_RRQ   ((uint16_t) 1)
#define TFTP_OPCODE_WRQ   ((uint16_t) 2)
#define TFTP_OPCODE_DATA  ((uint16_t) 3)
#define TFTP_OPCODE_ACK   ((uint16_t) 4)
#define TFTP_OPCODE_ERROR ((uint16_t) 5)

#define UDP_HEADER_SIZE      (8)
#define TFTP_OPCODE_SIZE     (2)
#define TFTP_BLOCKNO_SIZE    (2)
#define TFTP_DATA_SIZE       (512)
#define TFTP_MAX_PAYLOAD     (512)
#define TFTP_PACKET_MAX_SIZE (UDP_HEADER_SIZE+TFTP_OPCODE_SIZE+TFTP_BLOCKNO_SIZE+TFTP_MAX_PAYLOAD)

#define TFTP_PACKET_DELAY 400

// TFTP operation is not supported. The bootloader only supports 'put'
#define TFTP_NOT_SUPPORTED_ERROR_PACKET_LENGTH 18
const unsigned char tftp_not_supported_error_packet[TFTP_NOT_SUPPORTED_ERROR_PACKET_LENGTH] = "\0\5" "\0\4" "Not supported";

// Binary image file is larger than the available space
#define TFTP_FULL_ERROR_PACKET_LENGTH 9
const unsigned char tftp_full_error_packet[TFTP_FULL_ERROR_PACKET_LENGTH] = "\0\5" "\0\3" "Full";

// Passwords don't match
#define TFTP_PASSWORD_ERROR_PACKET_LENGTH 19
const unsigned char tftp_password_error_packet[TFTP_PASSWORD_ERROR_PACKET_LENGTH] = "\0\5" "\0\2" "Authentication";

// The CRC of the flashed image doesn't match
#define TFTP_CRC_ERROR_PACKET_LENGTH 16
const unsigned char tftp_crc_error_packet[TFTP_CRC_ERROR_PACKET_LENGTH] = "\0\5" "\0\0" "Image check";

// General catch-all error for unknown errors
#define TFTP_UNKNOWN_ERROR_PACKET_LENGTH 10
const unsigned char tftp_unknown_error_packet[TFTP_UNKNOWN_ERROR_PACKET_LENGTH] = "\0\5" "\0\0" "Error";

// Image file is not valid: the file is malformed or corrupted
#define TFTP_BAD_IMAGE_ERROR_PACKET_LENGTH 14
const unsigned char tftp_bad_image_error_packet[TFTP_BAD_IMAGE_ERROR_PACKET_LENGTH] = "\0\5" "\0\0" "Bad image";

static uint16_t lastBlockNumber = 0, highestBlockNumber = 0;
static uint8_t receivedRequest = TFTP_RXRQ_NONE;

#define TFTP_DATA_BUFFER_SIZE (TFTP_MAX_PAYLOAD * 2)

typedef struct _DataBuffer
{
   uint8_t buffer[TFTP_DATA_BUFFER_SIZE];
   uint8_t *head;
   uint8_t *tail;
   uint16_t count;
} DataBuffer;

DataBuffer tftpDataBuffer;

static void tftpDataBufferClear (void)
{
   tftpDataBuffer.count = 0U;
   tftpDataBuffer.head = tftpDataBuffer.tail = tftpDataBuffer.buffer;
}

static bool tftpDataBufferAddByte (uint8_t byteToAdd)
{
   if (tftpDataBuffer.count == TFTP_DATA_BUFFER_SIZE)
      return false;

   *(tftpDataBuffer.head++) = byteToAdd;
   ++tftpDataBuffer.count;

   if (tftpDataBuffer.head == tftpDataBuffer.buffer + TFTP_DATA_BUFFER_SIZE)
      tftpDataBuffer.head = tftpDataBuffer.buffer;

   return true;
}

static bool tftpDataBufferRemoveByte (uint8_t *removedByte)
{
   if (tftpDataBuffer.count == 0)
      return false;

   *removedByte = *(tftpDataBuffer.tail++);
   --tftpDataBuffer.count;

   if (tftpDataBuffer.tail == tftpDataBuffer.buffer + TFTP_DATA_BUFFER_SIZE)
      tftpDataBuffer.tail = tftpDataBuffer.buffer;

   return true;
}

uint16_t tftpReceivedBytesCount (void)
{
   return tftpDataBuffer.count;
}

bool tftpGetReceivedBytes (uint8_t *bytesBuffer, uint16_t bytesNumber)
{
   if (bytesNumber > tftpDataBuffer.count)
      // Not received enough data yet
      return false;

   while (bytesNumber--)
     tftpDataBufferRemoveByte(bytesBuffer++);

   return true;
}

bool tftpGetReceivedWords (uint8_t *bytesBuffer, uint16_t wordsNumber)
{
   return tftpGetReceivedBytes(bytesBuffer, wordsNumber << 2);
}

void tftpInit (void)
{
   netInit();
}

bool tftpIsReady (void)
{
   return netIsReady();
}

bool tftpConfigure (void)
{
   if (netConfigure())
   {
      receivedRequest = TFTP_RXRQ_NONE;
      netInitUdpSocket3(TFTP_PORT);
      tftpDataBufferClear();

      return true;
   }

   netEnd();

   return false;
}

void tftpEnd (void)
{
   netCloseSocket3();

   receivedRequest = TFTP_RXRQ_NONE;

   netEnd();
}

uint8_t tftpReceivedRequest (void)
{
   return receivedRequest;
}

bool tftpSendResponse (int8_t status)
{
   if (status == TFTP_STATUS_NO_TRAFFIC)
      // TFTP continues
      return true;

   uint8_t txBuffer[100];
   uint8_t* txPtr = txBuffer;
   uint16_t packetSize;

   switch (status)
   {
      default:

      case TFTP_STATUS_ERROR_UNKNOWN :
         packetSize = TFTP_UNKNOWN_ERROR_PACKET_LENGTH;
         memcpy(txBuffer, tftp_unknown_error_packet, (size_t) packetSize);
         break;

      case TFTP_STATUS_ERROR_NOT_SUPPORTED :
         packetSize = TFTP_NOT_SUPPORTED_ERROR_PACKET_LENGTH;
         memcpy(txBuffer, tftp_not_supported_error_packet, (size_t) packetSize);
         break;

      case TFTP_STATUS_ERROR_FULL :
         packetSize = TFTP_FULL_ERROR_PACKET_LENGTH;
         memcpy(txBuffer, tftp_full_error_packet, (size_t) packetSize);
         break;

      case TFTP_STATUS_ERROR_BAD_IMAGE :
         packetSize = TFTP_BAD_IMAGE_ERROR_PACKET_LENGTH;
         memcpy(txBuffer, tftp_bad_image_error_packet, (size_t) packetSize);
         break;

      case TFTP_STATUS_ERROR_PASSWORD :
         packetSize = TFTP_PASSWORD_ERROR_PACKET_LENGTH;
         memcpy(txBuffer, tftp_password_error_packet, (size_t) packetSize);
         break;

      case TFTP_STATUS_ERROR_CRC :
         packetSize = TFTP_CRC_ERROR_PACKET_LENGTH;
         memcpy(txBuffer, tftp_crc_error_packet, (size_t) packetSize);
         break;

      case TFTP_STATUS_SEND_ACK:
         if (highestBlockNumber < lastBlockNumber)
            highestBlockNumber = lastBlockNumber;
      case TFTP_STATUS_RX_COMPLETED:
         packetSize = 4;
         *(txPtr++) = TFTP_OPCODE_ACK >> 8;
         *(txPtr++) = TFTP_OPCODE_ACK & 0xff;
         *(txPtr++) = lastBlockNumber >> 8;
         *txPtr = lastBlockNumber & 0xff;
         break;
   }

   netSendPacketSocket3(txBuffer, packetSize);

   if (TFTP_IS_RX_COMPLETED(status) || TFTP_IS_ERROR_STATUS(status))
   {
      netCloseSocket3();

      // TFTP complete
      return false;
   }

   // TFTP continues
   return true;
}

static int8_t tftpProcessPacket (void)
{
   uint8_t buffer[TFTP_PACKET_MAX_SIZE];

   netSync(TFTP_PACKET_DELAY);

   netReceivePacketSocket3(buffer, TFTP_PACKET_MAX_SIZE);

   // Parse packet
   uint16_t udpDataLen = (buffer[6] << 8) + buffer[7];
   uint16_t tftpOpcode = (buffer[8] << 8) + buffer[9];
   uint16_t tftpBlockNumber = (buffer[10] << 8) + buffer[11];
   uint8_t *tftpData = buffer + 12;

   int8_t status = TFTP_STATUS_ERROR_UNKNOWN;
   uint16_t tftpDataLength;

   switch (tftpOpcode)
   {
      case TFTP_OPCODE_RRQ :
         // Read request
         receivedRequest = TFTP_RXRQ_RRQ;

         status = TFTP_STATUS_ERROR_NOT_SUPPORTED;

         break;

      case TFTP_OPCODE_WRQ :
         // Write request
         receivedRequest = TFTP_RXRQ_WRQ;

         netInitUdpSocket3(netcfgData.tftpDataPort);

         // Prepare for TFTP block number 0 acknowledge
         lastBlockNumber = highestBlockNumber = 0;
         status = TFTP_STATUS_SEND_ACK;

         break;

      case TFTP_OPCODE_DATA:
         tftpDataLength = udpDataLen - (TFTP_OPCODE_SIZE + TFTP_BLOCKNO_SIZE);
         lastBlockNumber = tftpBlockNumber;

         status = (tftpDataLength < TFTP_DATA_SIZE ? TFTP_STATUS_RX_COMPLETED : TFTP_STATUS_SEND_ACK);

         for (uint16_t idx = 0; idx < tftpDataLength; ++idx)
            if (!tftpDataBufferAddByte(*(tftpData++)))
            {
               status = TFTP_STATUS_ERROR_FULL;
               break;
            }

         break;

      // Acknowledgment
      case TFTP_OPCODE_ACK:

         receivedRequest = TFTP_RXRQ_NONE;

         status = TFTP_STATUS_ERROR_NOT_SUPPORTED;

         break;

      // Error signal
      case TFTP_OPCODE_ERROR:

         receivedRequest = TFTP_RXRQ_NONE;

         break;

      default:
         receivedRequest = TFTP_RXRQ_NONE;

         netInitUdpSocket3(netcfgData.tftpDataPort);

         // Just ignore the unexpected opcode
         status = TFTP_STATUS_NO_TRAFFIC;

         break;
   }

   return status;
}

int8_t tftpRun (void)
{
   if (netReceivedDataSizeSocket3())
      return tftpProcessPacket();

   return TFTP_STATUS_NO_TRAFFIC;
}
