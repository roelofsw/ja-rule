Memory Organization  {#memory}
===============================

[TOC]

# Overview {#memory-overview}

This page describes the layout of the different memory regions, since the
use of a bootloader makes this more complicated than the usual PIC32
program.

As background, read the chapter on [Memory Organization](
(http://ww1.microchip.com/downloads/en/DeviceDoc/60001115H.pdf)
from the PIC32 Family Reference Manual. It's important to
understand the difference between virtual and physical memory addresses, the
different regions (Boot vs Program Flash vs RAM) and the
difference between kseg0 and kseg1. We don't use kuseg so you won't see
virtual addresses less than 0x8000000.

It's also worth reading the [PIC32 Bootloader example]
(http://ww1.microchip.com/downloads/en/AppNotes/01388B.pdf)
since our memory layout is based on the one described there.

This page assumes a MX795F512L, since that's what is used on the Ethernet
Starter Kit. If you're using another chip you'll need to adjust the linker
scripts & the boot loader constants according to the amount of program flash
/ RAM provided on the chip.

Hardware constraints that influence the design include:
- Flash must be erased before programming (write), after an erase the flash
  contains all 1s.
- Flash can only be erased a page at a time (4kB chunks)
- Flash has a limited (rated for 1000) number of write cycles
- Flash is written either 4 bytes at a time, or 512B at a time.
- While writing flash, the program execution stalls.

# Memory Organization {#memory-organization}

## Boot / Program Flash {#memory-organization-flash}

The MX795F512L has the following (physical) memory regions:
 - 128kB of RAM [0x00000000 - 0x0001ffff]
 - 12kB of Boot flash 0x1fc00000 - 0x1fc02fef]. The upper 4k of the boot
   flash is used for the debug executive (in circuit debugging and the
   Device Configuration Registers.
 - 512kB of Program Flash [0x1d000000 - 0x1d07ffff]
 - SFRs, including peripherals, which aren't important for this topic.

Since our bootloader (16k) is too big to fit in the boot flash we place it
in the lower part of the program flash. However the boot flash is still used
for the initial code (.reset) and the interrupt vector table (IVT). This
minimises the amount of program flash used by the bootloader.

Since the early models don't have an EEPROM, we also store the device's UID
in program flash. We could have embedded the UID in the bootloader but this
means you need a custom bootloader image for each device.

The downside of storing the UID in program flash is that we consume an
entire page (4kB) for a 6 byte UID.

The main application comes with it's own IVT that we need to reserve room
for. We put this in the first 4k of the application's address range, and
then the program code follows it.

Putting this all together, the Program Flash layout is:

Start Address  | End Address   | Size  | Use
-------------- | ------------- | ----- | ----------
0x9d000000     | 0x9d005fff    | 24kB  | Bootloader
0x9d006000     | 0x9d006fff    | 4kB   | UID
0x9d007000     | 0x9d007fff    | 4kB   | Application IVT
0x9d008000     | 0x9d07ffff    | 479kB | Application Code

Finally, we use the Program Flash Write Protect (PWP, see the data sheet for
the chip) feature to avoid a bug accidently overwriting the bootloader.
PWP is at the page-level so we write protect the first 6 pages (24k).

## RAM {#memory-organization-ram}

During the firmware update procedure, the application needs a way to signal
to the initial code that it should enter the bootloader rather than boot the
main application. We call this the <em>boot_option</em>.

We could use flash for this, but we'd have to dedicate another
entire page of flash (4kB) since that's the smallest granularity of a
flash-erase and it would persist accross power-cycles, which may not be what
we want.

Instead, we store the Boot Options in the upper range of the RAM address
space. To enter the bootloader, the application writes the desired boot
option to the boot_option memory location and then triggers a software
reset.

Start Address  | End Address   | Size   | Use
-------------- | ------------- | ------ | ----------
0xa0000000     | 0xa001ffef    | ~127kB | General RAM
0xa001fff0     | 0xa001ffff    | 16B    | Bootloader Options

We reserved 16B in case we need to transfer any other data between
the bootloader and the application.

# Linker Scripts {#memory-linker}

The linker is responsible for taking the object files (.o) and combining
them to form a single executable. The linker configuration is specified
through the use of a linker script, which controls the mapping of data to
memory regions.

The [MPLAB® Assembler, Linker and Utilities for PIC32 MCUs User’s Guide]
(http://ww1.microchip.com/downloads/en/DeviceDoc/51833A.pdf)
covers the different types of memory sections and the use of linker scripts.

We have a custom linker script for both the bootloader and the application.
These can be found in the linker directory.

# References {#memory-references}

- [Section 3, Memory Organization]
  (http://ww1.microchip.com/downloads/en/DeviceDoc/60001115H.pdf)
- [PIC32MX5XX/6XX/7XX Data sheet]
  (http://ww1.microchip.com/downloads/en/DeviceDoc/61156H.pdf)
- [PIC32 Bootloader example]
  (http://ww1.microchip.com/downloads/en/AppNotes/01388B.pdf)
- [MPLAB® Assembler, Linker and Utilities for PIC32 MCUs User’s Guide]
  (http://ww1.microchip.com/downloads/en/DeviceDoc/51833A.pdf)
  MPLAB® Assembler, Linker and Utilities for PIC32 MCUs User’s Guide</a>
