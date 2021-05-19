/** @defgroup adc_defines Analog / Digital Converter Defines
 *
 * @brief <b>Defined Constants and Types for the LM4F General Purpose I/O</b>
 *
 * @ingroup LM4Fxx_defines
 *
 * @version 1.0.0
 *
 * @author @htmlonly &copy; @endhtmlonly 2011
 * Gareth McMullin <gareth@blacksphere.co.nz>
 * @author @htmlonly &copy; @endhtmlonly 2013
 * Alexandru Gagniuc <mr.nuke.me@gmail.com>
 *
 * @date 16 March 2013
 *
 * LGPL License Terms @ref lgpl_license
 */

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2011 Gareth McMullin <gareth@blacksphere.co.nz>
 * Copyright (C) 2013 Alexandru Gagniuc <mr.nuke.me@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LM4F_GPIO_H
#define LM4F_GPIO_H

/**@{*/

#include <libopencm3/cm3/common.h>
#include <libopencm3/lm4f/memorymap.h>

// will be add to memorymap.h...
#define ADC0_BASE			(0x40038000U)
#define ADC1_BASE			(0x40039000U)

/* =============================================================================
 * Convenience macros
 * ---------------------------------------------------------------------------*/
/** @defgroup adc_reg_base GPIO register base addresses
 * @{*/
#define ADC0				ADC0_BASE
#define ADC1				ADC1_BASE
/** @} */

/* =============================================================================
 * Function prototypes
 * ---------------------------------------------------------------------------*/
BEGIN_DECLS

void adc_init(uint32_t port, uint8_t pin);

END_DECLS

#endif

/**@}*/


