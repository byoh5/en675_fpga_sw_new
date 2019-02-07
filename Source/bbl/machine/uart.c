// See LICENSE for license details.

#include <string.h>
#include "uart.h"
#include "fdt.h"

volatile uint32_t* uart = 0;

void uart_putchar(uint8_t ch)
{
  volatile uint32_t *tx = uart + UART_REG_TXFIFO;

  while (uart[UART_REG_CTRL]&UART_TX_FULL) {;}
  *tx = ((uint32_t)ch) << 8 ;
}

int uart_getchar()
{
  if (uart[UART_REG_CTRL]&UART_RX_EMPTY) return -1;
  else return (int) (uart[UART_REG_RXFIFO]&0xFF);
}

struct uart_scan
{
  int compat;
  uint64_t reg;
};

static void uart_open(const struct fdt_scan_node *node, void *extra)
{
  struct uart_scan *scan = (struct uart_scan *)extra;
  memset(scan, 0, sizeof(*scan));
}

static void uart_prop(const struct fdt_scan_prop *prop, void *extra)
{
  struct uart_scan *scan = (struct uart_scan *)extra;
  if (!strcmp(prop->name, "compatible") && !strcmp((const char*)prop->value, "eyenix,uart0")) {
    scan->compat = 1;
  } else if (!strcmp(prop->name, "reg")) {
    fdt_get_address(prop->node->parent, prop->value, &scan->reg);
  }
}

static void uart_done(const struct fdt_scan_node *node, void *extra)
{
  struct uart_scan *scan = (struct uart_scan *)extra;
  if (!scan->compat || !scan->reg || uart) return;

  // Enable Rx/Tx channels
  uart = (void*)(uintptr_t)scan->reg;
  uart[UART_REG_CTRL] = 0x01a02040;		// Baud rate, TX type
}

void query_uart(uintptr_t fdt)
{
  struct fdt_cb cb;
  struct uart_scan scan;

  memset(&cb, 0, sizeof(cb));
  cb.open = uart_open;
  cb.prop = uart_prop;
  cb.done = uart_done;
  cb.extra = &scan;

  fdt_scan(fdt, &cb);
}
