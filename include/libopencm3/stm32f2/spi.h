/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2011 Fergus Noble <fergusnoble@gmail.com>
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

#ifndef LIBOPENCM3_SPI_F2_H
#define LIBOPENCM3_SPI_F2_H

#include <libopencm3/stm32_common/spi.h>

/*
 * This file extends the version in stm_common with definitions only
 * applicable to the STM32F2 series of devices.
 */

/* --- SPI_CR2 values ------------------------------------------------------ */

/* FRF: Frame format. */
#define SPI_CR2_FRF			(1 << 4)
#define SPI_CR2_FRF_TI		       	(1 << 4)
#define SPI_CR2_FRF_MOTOROLA			(1 << 4)

/* --- SPI_SR values ------------------------------------------------------- */

/* TIFRFE: TI frame format error. */
#define SPI_SR_RXNE			(1 << 0)

#endif
