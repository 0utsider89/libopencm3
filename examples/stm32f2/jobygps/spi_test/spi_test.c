/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 *               2011 Piotr Esden-Tempski <piotr@esden.net>
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

#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/f2/gpio.h>
#include <libopencm3/stm32/f2/rcc.h>

#include <stdio.h>
#include <errno.h>

void clock_setup(void)
{
	//rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOA clock (for LED GPIOs). */
	//rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	//rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN |
//				RCC_APB2ENR_AFIOEN |
//				RCC_APB2ENR_USART1EN);
  RCC_APB1ENR |= RCC_APB1ENR_SPI2EN;
  RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;   // PORT B not enabled...
	
}

void spi_setup(void)
{
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

	/* Setup SPI parameters. */
  spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_256, SPI_CR1_CPOL, SPI_CR1_CPHA, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

	/* Finally enable the SPI. */
	spi_enable(SPI2);
}

void usart_setup(void)
{
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
	//GPIO_MODER(GPIOA) |= GPIO_MODE(9, GPIO_MODE_AF);
	//gpio_set_af(GPIOA, GPIO_AF7, GPIO9|GPIO10);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 9600, 16000000);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

void gpio_setup(void)
{
	gpio_set(GPIOC, GPIO3);

	/* Setup GPIO6 and 7 (in GPIO port A) for led use. */
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT,
		      GPIO_MODE_OUTPUT, GPIO3);
}

int _write (int file, char *ptr, int len)
{
        int i;

	if (file == 1) {
		for (i = 0; i < len; i++){
			usart_send_blocking(USART1, ptr[i]);
		}

		return i;
	}

        errno = EIO;
        return -1;
}

int main(void)
{
	int counter = 0;

	clock_setup();
	gpio_setup();
	usart_setup();
  spi_setup();

	/*
	 * Write Hello World an integer, float and double all over
	 * again while incrementing the numbers.
	 */
	while (1) {
		gpio_toggle(GPIOC, GPIO3);
		printf("Hello World! %i\r\n", counter);
		counter++;
    spi_send(SPI2,(u8)counter);
	}

	return 0;
}
