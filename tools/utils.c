/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Copyright (C) 2015 Simon Newton.
 */

#include "utils.h"

#include <stdlib.h>

bool StringToUInt16(const char *input, uint16_t *output) {
  long i = strtol(input, NULL, 0);
  if (i < 0 || i > UINT16_MAX) {
    return false;
  }
  *output = (uint16_t) i;
  return true;
}

bool StringToUInt32(const char *input, uint32_t *output) {
  long i = strtol(input, NULL, 0);
  if (i < 0 || i > UINT32_MAX) {
    return false;
  }
  *output = (uint32_t) i;
  return true;
}