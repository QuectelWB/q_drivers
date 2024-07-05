/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Definitions for USB serial mobile broadband cards
 */

#ifndef __LINUX_USB_USB_WWAN
#define __LINUX_USB_USB_WWAN
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,7,0)
static inline bool tty_port_initialized(struct tty_port *port)
{
	return test_bit(ASYNCB_INITIALIZED, &port->flags);
}
#endif
extern void usb_wwan_dtr_rts(struct usb_serial_port *port, int on);
extern int usb_wwan_open(struct tty_struct *tty, struct usb_serial_port *port);
extern void usb_wwan_close(struct usb_serial_port *port);
extern int usb_wwan_port_probe(struct usb_serial_port *port);
#if LINUX_VERSION_CODE > KERNEL_VERSION(5,12,0)
extern void usb_wwan_port_remove(struct usb_serial_port *port);
#else
extern int usb_wwan_port_remove(struct usb_serial_port *port);
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(5,14,0)
extern unsigned int usb_wwan_write_room(struct tty_struct *tty);
extern unsigned int usb_wwan_chars_in_buffer(struct tty_struct *tty);
#else
extern int usb_wwan_write_room(struct tty_struct *tty);
extern int usb_wwan_chars_in_buffer(struct tty_struct *tty);
#endif
extern int usb_wwan_tiocmget(struct tty_struct *tty);
extern int usb_wwan_tiocmset(struct tty_struct *tty,
			     unsigned int set, unsigned int clear);
extern int usb_wwan_write(struct tty_struct *tty, struct usb_serial_port *port,
			  const unsigned char *buf, int count);
#ifdef CONFIG_PM
extern int usb_wwan_suspend(struct usb_serial *serial, pm_message_t message);
extern int usb_wwan_resume(struct usb_serial *serial);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,13,0)
extern int usb_wwan_get_serial_info(struct tty_struct *tty,
                          struct serial_struct *ss);
extern int usb_wwan_set_serial_info(struct tty_struct *tty,
                          struct serial_struct *ss);
#endif

/* per port private data */

#define N_IN_URB 4
#define N_OUT_URB 4
#define IN_BUFLEN 4096
#define OUT_BUFLEN 4096

struct usb_wwan_intf_private {
	spinlock_t susp_lock;
	unsigned int suspended:1;
	unsigned int use_send_setup:1;
	unsigned int use_zlp:1;
	int in_flight;
	unsigned int open_ports;
	void *private;
};

struct usb_wwan_port_private {
	/* Input endpoints and buffer for this port */
	struct urb *in_urbs[N_IN_URB];
	u8 *in_buffer[N_IN_URB];
	/* Output endpoints and buffer for this port */
	struct urb *out_urbs[N_OUT_URB];
	u8 *out_buffer[N_OUT_URB];
	unsigned long out_busy;	/* Bit vector of URBs in use */
	struct usb_anchor delayed;

	/* Settings for the port */
	int rts_state;		/* Handshaking pins (outputs) */
	int dtr_state;
	int cts_state;		/* Handshaking pins (inputs) */
	int dsr_state;
	int dcd_state;
	int ri_state;

	unsigned long tx_start_time[N_OUT_URB];
};

#endif /* __LINUX_USB_USB_WWAN */
