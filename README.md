# Industruino Core for SAMD21 CPU

This repository contains the source code and configuration files of the Industruino Core
for Atmel's SAMD21 processor (used on the Industruino D21G board).

This core is based on the [Arduino Core for SAMD21 CPU](https://github.com/arduino/ArduinoCore-samd).

The bootloader TFTP remote upload feature is based on the [Ariadne Bootloader for Arduino](https://github.com/codebendercc/Ariadne-Bootloader) code.

## Installation on Arduino IDE

This core can be installed as a package by the Arduino IDE cores manager.

Add the following URL to the "Additional Boards Manager URLs" field of the Arduino IDE preferences

https://static.industruino.com/downloads/code/IndustruinoCores/IndustruinoSAMD/pkgdef/package_industruino_samd_index.json

then open the "Boards Manager" and install the package called "Industruino SAMD Boards (32-bit ARM Cortex-M0+)"

## Support

There is a dedicated post on the Industruino forum for project assistance regarding the D21G board:

https://industruino.com/forum/help-1/question/d21g-board-migration-help-325

## Bugs or Issues

If you find a bug you can submit an issue here on GitHub:

https://github.com/Industruino/IndustruinoSAMD/issues

Before posting a new issue, please check if the same problem has been already reported by someone else
to avoid duplicates.

## Contributions

Contributions are always welcome. The preferred way to receive code cotribution is by submitting a
Pull Request on GitHub.

## License and credits

This core has been originally developed by Arduino LLC in collaboration with Atmel.

The bootloader TFTP remote upload feature has been originally delevoped for the AVR-based Arduino boards by Codebender Inc. ([Ariadne Bootloader](https://github.com/codebendercc/Ariadne-Bootloader)).

```
  Copyright (c) 2015 Arduino LLC.  All right reserved.
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
```
