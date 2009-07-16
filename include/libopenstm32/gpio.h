/*
 * This file is part of the libopenstm32 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef LIBOPENSTM32_GPIO_H
#define LIBOPENSTM32_GPIO_H

#define GPIO_CTRL_LO		0x00
#define GPIO_CTRL_HI		0x04
#define GPIO_INPUT_DATA		0x08
#define GPIO_OUTPUT_DATA	0x0c
#define GPIO_BIT_SET_RESET	0x10
#define GPIO_BIT_RESET		0x14
#define GPIO_LOCK		0x18

/* TODO */

void gpio_set(int gpio);
void gpio_clear(int gpio);
void gpio_toggle(int gpio);

#endif
