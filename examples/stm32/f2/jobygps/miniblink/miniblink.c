/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/f2/gpio.h>

void gpio_setup(void)
{
	/* Enable GPIOC clock. */
	/* Manually: */
	// RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;
	/* Using API functions: */
	//rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

	/* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
	/* Manually: */
	// GPIOC_CRH = (GPIO_CNF_OUTPUT_PUSHPULL << (((12 - 8) * 4) + 2));
	// GPIOC_CRH |= (GPIO_MODE_OUTPUT_2_MHZ << ((12 - 8) * 4));
	/* Using API functions: */
	MMIO32(RCC_BASE + 0x30) |= (1 << 2);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3 | GPIO4);
}

int main(void)
{
	int i;

	gpio_setup();

	/* Blink the LED (PC12) on the board. */
	while (1) {
		/* Manually: */
		// GPIOC_BSRR = GPIO12;		/* LED off */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		// 	__asm__("nop");
		// GPIOC_BRR = GPIO12;		/* LED on */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		// 	__asm__("nop");

		/* Using API functions gpio_set()/gpio_clear(): */
		// gpio_set(GPIOC, GPIO12);	/* LED off */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		// 	__asm__("nop");
		// gpio_clear(GPIOC, GPIO12);	/* LED on */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		// 	__asm__("nop");

		/* Using API function gpio_toggle(): */
		//gpio_toggle(GPIOC, GPIO3);	/* LED on/off */
		gpio_set(GPIOC, GPIO3);
		gpio_clear(GPIOC, GPIO4);
		for (i = 0; i < 800000; i++)	/* Wait a bit. */
			__asm__("nop");
		gpio_clear(GPIOC, GPIO3);
		gpio_set(GPIOC, GPIO4);
		for (i = 0; i < 800000; i++)	/* Wait a bit. */
			__asm__("nop");
	}

	return 0;
}
