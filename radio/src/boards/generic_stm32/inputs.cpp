/*
 * Copyright (C) EdgeTx
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "hal/key_driver.h"

#include "stm32_hal_ll.h"
#include "stm32_gpio_driver.h"

#include "stm32_keys.inc"

#define __weak __attribute__((weak))

__weak void keysInit()
{
  _init_keys();
  _init_trims();
}

__weak uint32_t readKeys()
{
  #if defined(RADIO_PETX)
  {
     if ((_read_trims() & 0x01) || (_read_trims() & 0x02))
        return 0;
    else  
         return _read_keys();
  }
  #else
  return _read_keys();
 #endif
}

__weak uint32_t readTrims()
{
  uint32_t trims = _read_trims();
#if defined(RADIO_PETX)
  {
    trims &= 0xF00;
    if (_read_trims() == 0x03)
     {
        trims |= 0x42;//返回BOOTLOADER_KEYS
     }
    else if (_read_trims() & 0x01)
     {
      if (_read_keys() & (1 << KEY_LEFT))
         trims |= 0x01;
      if (_read_keys() & (1 << KEY_RIGHT))
         trims |= 0x02;
      if (_read_keys() & (1 << KEY_DOWN))
         trims |= 0x04;
      if (_read_keys() & (1 << KEY_UP))
         trims |= 0x08;       
     }
   else if (_read_trims() & 0x02)
    {
      if (_read_keys() & (1 << KEY_LEFT))
         trims |= 0x40;
      if (_read_keys() & (1 << KEY_RIGHT))
         trims |= 0x80;
      if (_read_keys() & (1 << KEY_DOWN))
         trims |= 0x10;
      if (_read_keys() & (1 << KEY_UP))
         trims |= 0x20;       
    }
  }
#endif
  return trims;
}
