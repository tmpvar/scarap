#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "sp.h"

#define TX_BUFFER_SIZE 128

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

volatile char tx_buf[TX_BUFFER_SIZE];
volatile uint8_t tx_write_location, tx_read_location;

void sp_emit(char c, FILE *stream) {
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c; 
}

static FILE mystdout = FDEV_SETUP_STREAM(sp_emit, NULL, _FDEV_SETUP_WRITE);

void sp_init() {
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1<<RXCIE0);// | (1 << TXCIE0); // Turn on the transmission and reception circuitry
  UCSR0C = /*(1 << UMSEL00) |*/ (1 << UCSZ01) | (1 << UCSZ00); // Use 8-bit character sizes
  UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
  UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

  tx_write_location = 0;
  tx_read_location = 0;

  stdout = &mystdout;
}

void (*read_callback)(char) = 0;

void sp_read_callback(void (*cb)(char)) {
  read_callback = cb;
}

ISR(USART_RX_vect) {
  if (read_callback) {
    read_callback(UDR0);
  }
}
