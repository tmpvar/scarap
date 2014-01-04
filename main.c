/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <util/delay.h>

#include "uart.h"
#include "gcode.h"


void sp_emit(char c, FILE *stream) {
  uart0_putc(c);
}


static FILE mystdout = FDEV_SETUP_STREAM(sp_emit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
  wdt_disable();
  cli();

  // Stepper enable
  DDRB |= (1<<0);
  PORTB |= (1<<0);

  // Z axis step and direction
  DDRD |= (1<<4) | (1<<7);
  // Y axis step and direction
  DDRD |= (1<<3) | (1<<6);
  // X axis step and direction
  DDRD |= (1<<2) | (1<<5);
  PORTB &= ~(1<<0);
  stdout = &mystdout;

  uart0_init(UART_BAUD_SELECT(9600, F_CPU));

  sei();

  printf("scarap 0.0.1\n");
  char c;
  unsigned long i = 0;
  for(;;) {
    if (uart0_available()) {
      c = uart0_getc();
      if (c > 0 && c != ' ' && c != '\r') {
        gcode_push(c);
      }
    }
  }

  return 0;   /* never reached */
}
