/** @file
 *
 * Definitions for handling vector tables.
 *
 * This implements d0002_efm32_cortex-m3_reference_manual.pdf's figure 2.2.
 *
 * The structure of the vector table is implemented independently of the system
 * vector table starting at memory position 0x0, as it can be relocated to
 * other memory locations too.
 *
 * The exact size of a vector interrupt table depends on the number of
 * interrupts IRQ_COUNT, which is defined per family.
 */

#ifndef LIBOPENCM3_EFM32_VECTOR_H
#define LIBOPENCM3_EFM32_VECTOR_H

#include <libopencm3/cm3/common.h>

#ifdef TINYGECKO
#	include <libopencm3/efm32/tinygecko/irq.h>
#else
#	error "efm32 family not defined."
#endif

/** Type of an interrupt function. Only used to avoid hard-to-read function
 * pointers in the efm32_vector_table_t struct. */
typedef void (*efm32_vector_table_entry_t)(void);

typedef struct {
	unsigned int *initial_sp_value; /**< The value the stack pointer is set to initially */
	efm32_vector_table_entry_t reset;
	efm32_vector_table_entry_t nmi;
	efm32_vector_table_entry_t hard_fault;
	efm32_vector_table_entry_t memory_manage_fault;
	efm32_vector_table_entry_t bus_fault;
	efm32_vector_table_entry_t usage_fault;
	efm32_vector_table_entry_t reserved_x001c[4];
	efm32_vector_table_entry_t sv_call;
	efm32_vector_table_entry_t reserved_debug;
	efm32_vector_table_entry_t reserved_x0034;
	efm32_vector_table_entry_t pend_sv;
	efm32_vector_table_entry_t systick;
	efm32_vector_table_entry_t irq[IRQ_COUNT];
} efm32_vector_table_t;

#endif
