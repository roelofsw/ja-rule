/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * rdm_util.c
 * Copyright (C) 2015 Simon Newton
 */
#include "rdm_util.h"

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#include "system_pipeline.h"
#include "constants.h"
#include "utils.h"

static uint16_t Checksum(const uint8_t *data, unsigned int length) {
  uint16_t checksum = 0;
  unsigned int i;
  for (i = 0; i < length; i++) {
    checksum += data[i];
  }
  return checksum;
}

bool RDMUtil_RequiresAction(const RDMResponder *responder,
                            const uint8_t uid[UID_LENGTH]) {
  if (RDMUtil_UIDCompare(responder->uid, uid) == 0) {
    return true;
  }

  if (uid[2] != 0xff || uid[3] != 0xff || uid[4] != 0xff || uid[5] != 0xff) {
    // definitely not a broadcast
    return false;
  }

  return (uid[0] == responder->uid[0] && uid[1] == responder->uid[1]) ||
         (uid[0] == 0xff && uid[1] == 0xff);
}

bool RDMUtil_VerifyChecksum(const uint8_t *frame, unsigned int size) {
  if (size < sizeof(RDMHeader) + (unsigned int) RDM_CHECKSUM_LENGTH ||
      frame[2] + (unsigned int) RDM_CHECKSUM_LENGTH != size) {
    return false;
  }

  uint8_t message_length = frame[MESSAGE_LENGTH_OFFSET];
  uint16_t checksum = Checksum(frame, message_length);
  return (ShortMSB(checksum) == frame[message_length] &&
          ShortLSB(checksum) == frame[message_length + 1]);
}

int RDMUtil_AppendChecksum(uint8_t *frame) {
  uint8_t message_length = frame[MESSAGE_LENGTH_OFFSET];
  uint16_t checksum = Checksum(frame, message_length);
  frame[message_length] = ShortMSB(checksum);
  frame[message_length + 1] = ShortLSB(checksum);
  return message_length + RDM_CHECKSUM_LENGTH;
}

unsigned int RDMUtil_SafeStringLength(const char *str, unsigned int max_size) {
  unsigned int size = 0;
  while (*str++ && size < max_size) {
    size++;
  }
  return size;
}
