/*
 * This file is part of the libopenstm32 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
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

#include <libopenstm32/common.h>
#include <libopenstm32/tools.h>
#include <libopenstm32/usb.h>
#include <usb/usbd.h>
#include "usb_private.h"

/** Initialize USB Device Controller.
 *
 *  Function initializes the USB Device controller hardware 
 *  of the STM32 microcontroller.
 */
void _usbd_hw_init(void)
{

	SET_REG(USB_CNTR_REG, 0);
	SET_REG(USB_BTABLE_REG, 0);
	SET_REG(USB_ISTR_REG, 0);

	/* Enable RESET, SUSPEND, RESUME and CTR interrupts */
	SET_REG(USB_CNTR_REG, USB_CNTR_RESETM | USB_CNTR_CTRM |
				USB_CNTR_SUSPM | USB_CNTR_WKUPM);
}

void _usbd_hw_set_address(u8 addr)
{
	/* Set device address and enable. */
	SET_REG(USB_DADDR_REG, (addr & USB_DADDR_ADDR) | USB_DADDR_ENABLE);
}

/** Set the receive buffer size for a given USB endpoint 
 *  @param  ep     Index of Endpoint to configure
 *  @param  addr   Size in bytes of RX buffer
 */
static void usb_set_ep_rx_bufsize(u8 ep, u32 size)
{
	if(size > 62) {
		if(size & 0x1f) size -= 32;
		USB_SET_EP_RX_COUNT(ep, (size << 5) | 0x8000);
	} else {
		if(size & 1) size++;
		USB_SET_EP_RX_COUNT(ep, size << 10);
	}
}

void usbd_ep_setup(u8 addr, u8 type, u16 max_size, void (*callback)(u8 ep))   
{
	/* Translate USB standard type codes to stm32 */
	const u16 typelookup[] = {
		[USB_ENDPOINT_ATTR_CONTROL] = USB_EP_TYPE_CONTROL,
		[USB_ENDPOINT_ATTR_ISOCHRONOUS] = USB_EP_TYPE_ISO,
		[USB_ENDPOINT_ATTR_BULK] = USB_EP_TYPE_BULK,
		[USB_ENDPOINT_ATTR_INTERRUPT] = USB_EP_TYPE_INTERRUPT,
	};
	u8 dir = addr & 0x80;
	addr &= 0x7f;

	/* Assign address */
	USB_SET_EP_ADDR(addr, addr);
	USB_SET_EP_TYPE(addr, typelookup[type]);

	if(dir || (addr == 0)) {
		USB_SET_EP_TX_ADDR(addr, _usbd_device.pm_top);
		if(callback)
			_usbd_device.user_callback_ctr[addr][USB_TRANSACTION_IN] = (void*)callback;
		USB_CLR_EP_TX_DTOG(addr);
		USB_SET_EP_TX_STAT(addr, USB_EP_TX_STAT_NAK);
		_usbd_device.pm_top += max_size;
	} 
	if(!dir) {
		USB_SET_EP_RX_ADDR(addr, _usbd_device.pm_top);
		usb_set_ep_rx_bufsize(addr, max_size);
		if(callback)
			_usbd_device.user_callback_ctr[addr][USB_TRANSACTION_OUT] = (void*)callback;
		USB_CLR_EP_RX_DTOG(addr);
		USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);
		_usbd_device.pm_top += max_size;
	}
}

void _usbd_hw_endpoints_reset(void)
{
	int i;

	/* Reset all endpoints */
	for(i = 1; i < 8; i++) {
		USB_SET_EP_TX_STAT(i, USB_EP_TX_STAT_DISABLED);
		USB_SET_EP_RX_STAT(i, USB_EP_RX_STAT_DISABLED);
	}
	_usbd_device.pm_top = 0x40 + (2*_usbd_device.desc->bMaxPacketSize0);
}

void usbd_ep_stall_set(u8 addr, u8 stall)
{
	if(addr == 0)
		USB_SET_EP_TX_STAT(addr, 
			stall ? USB_EP_TX_STAT_STALL : USB_EP_TX_STAT_NAK);

	if(addr & 0x80) {
		addr &= 0x7F;

		USB_SET_EP_TX_STAT(addr, 
			stall ? USB_EP_TX_STAT_STALL : USB_EP_TX_STAT_NAK);

		/* Reset to DATA0 if clearing stall condition */
		if(!stall)
			USB_CLR_EP_TX_DTOG(addr);
	} else {
		/* Reset to DATA0 if clearing stall condition */
		if(!stall)
			USB_CLR_EP_RX_DTOG(addr);

		USB_SET_EP_RX_STAT(addr, 
			stall ? USB_EP_RX_STAT_STALL : USB_EP_RX_STAT_VALID);
	}
}

u8 usbd_ep_stall_get(u8 addr)
{
	if(addr & 0x80) {
		if ((*USB_EP_REG(addr & 0x7F) & USB_EP_TX_STAT) == 
						USB_EP_TX_STAT_STALL)
			return 1;
        } else {
		if ((*USB_EP_REG(addr) & USB_EP_RX_STAT) == 
						USB_EP_RX_STAT_STALL)
			return 1;
	}
	return 0;
}

/** Copy a data buffer to Packet Memory.
 *  @param PM     Destination pointer into packet memory.
 *  @param buf    Source pointer to data buffer.
 *  @param len    Number of bytes to copy.
 */
static inline void 
usb_copy_to_pm(volatile void *vPM, const void *buf, uint16_t len)
{
        const uint16_t *lbuf = buf;
	volatile uint16_t *PM = vPM;

        for(len = (len + 1) >> 1; len; PM += 2, lbuf++, len--)
                *PM = *lbuf;
}

u16 usbd_ep_write_packet(u8 addr, const void *buf, u16 len)
{
	addr &= 0x7F;

	if((*USB_EP_REG(addr) & USB_EP_TX_STAT) == USB_EP_TX_STAT_VALID) 
		return 0;

	usb_copy_to_pm(USB_GET_EP_TX_BUFF(addr), buf, len);
	USB_SET_EP_TX_COUNT(addr, len);
	USB_SET_EP_TX_STAT(addr, USB_EP_TX_STAT_VALID);

	return len;
}

/** Copy a data buffer from Packet Memory.
 *  @param buf    Source pointer to data buffer.
 *  @param PM     Destination pointer into packet memory.
 *  @param len    Number of bytes to copy.
 */
static inline void 
usb_copy_from_pm(void *buf, const volatile void *vPM, uint16_t len)
{
        uint16_t *lbuf = buf;
	const volatile uint16_t *PM = vPM;
        uint8_t odd = len & 1;

        for(len >>= 1; len; PM += 2, lbuf++, len--)
                *lbuf = *PM;

        if(odd) *(uint8_t*)lbuf = *(uint8_t*)PM;
}

u16 usbd_ep_read_packet(u8 addr, void *buf, u16 len)
{
	if((*USB_EP_REG(addr) & USB_EP_RX_STAT) == USB_EP_RX_STAT_VALID) 
		return 0;

	len = MIN(USB_GET_EP_RX_COUNT(addr) & 0x3ff, len);
	usb_copy_from_pm(buf, USB_GET_EP_RX_BUFF(addr), len);
	USB_CLR_EP_RX_CTR(addr);

	USB_SET_EP_RX_STAT(addr, USB_EP_RX_STAT_VALID);

	return len;
}

void usbd_poll(void)
{
	u16 istr = *USB_ISTR_REG;

	if(istr & USB_ISTR_RESET) {
		_usbd_device.pm_top = 0x40;
		_usbd_reset();
		USB_CLR_ISTR_RESET();
		return;
	} 

	if(istr & USB_ISTR_CTR) {
		u8 ep = istr & USB_ISTR_EP_ID;
		u8 type = (istr & USB_ISTR_DIR) ? 1 : 0;

		if(type) {	/* OUT or SETUP transaction */
			type += (*USB_EP_REG(ep) & USB_EP_SETUP) ? 1 : 0;
		} else {	/* IN transaction */
			USB_CLR_EP_TX_CTR(ep);
		}

		if(_usbd_device.user_callback_ctr[ep][type])
			_usbd_device.user_callback_ctr[ep][type](ep);
	}

	if(istr & USB_ISTR_SUSP) {
		USB_CLR_ISTR_SUSP();
		if(_usbd_device.user_callback_suspend)
			_usbd_device.user_callback_suspend();
	}

	if(istr & USB_ISTR_WKUP) {
		USB_CLR_ISTR_WKUP();
		if(_usbd_device.user_callback_resume)
			_usbd_device.user_callback_resume();
	}

	if(istr & USB_ISTR_SOF) 
		USB_CLR_ISTR_SOF();
}
