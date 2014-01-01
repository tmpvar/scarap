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
#include "sp.h"

void on_serial(char c) {
  if (c != ' ') {
    gcode_push(c);
    printf("%c", c);
  }
}

int main(void)
{
  wdt_disable();

  // Stepper enable
  DDRB |= (1<<0);
  PORTB |= (1<<0);

  // Z axis step and direction
  DDRD |= (1<<4) | (1<<7);
  // Y axis step and direction
  DDRD |= (1<<3) | (1<<6);
  // X axis step and direction
  DDRD |= (1<<2) | (1<<5);

  sp_init();

  sp_read_callback(on_serial);

  sei();

  printf("scarap 0.0.1\n");

  /* insert your hardware initialization here */
  for(;;){
    /* insert your main loop code here */
    //printf(".");
    _delay_ms(10);
  }
  printf("hrm\n");
  return 0;   /* never reached */
}
