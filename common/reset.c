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
 * reset.c
 * Copyright (C) 2015 Simon Newton
 */

#include "reset.h"


// TODO(simon): Remove this and switch to the harmony function
#define _SUPPRESS_PLIB_WARNING

#include <peripheral/reset/plib_reset.h>

void Reset_SoftReset() {
    PLIB_RESET_SoftwareResetEnable(RESET_ID_0);
}
