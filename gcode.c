#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "gcode.h"

void gcode_init() {

}

uint8_t mode = 0;
uint8_t coord = 0;

float parsed_result = 0;
uint8_t float_mode = 0;
long unsigned int float_position = 1;
int read_float(char c) {
  
  if (c == '.') {
    float_mode = 1;
    return 1;
  } else if (!isdigit(c)) {
    return 0;
  }

  switch (float_mode) {
    case 0:
      parsed_result *= 10;
      parsed_result += (c-'0');
    break;
    
    case 1:
      float_position *= 10;
      parsed_result += (float)(c-'0')/(float)float_position;
    break;
  }

  return 1;
}

void gcode_push(char c) {
  switch(mode) {
    case 0:
      switch (toupper(c)) {
        case 'X':
          coord = 1;
          mode = 1;
        break;

        case 'Y':
          coord = 2;
          mode = 1;
        break;

        case 'Z':
          coord = 3;
          mode = 1;
        break;
      }
    break;

    case 1: // parse float
      if (!read_float(c)) {
        mode = 0;
        printf(": %f; %c\n", parsed_result, c);
        // reset the float parser
        parsed_result = 0;
        float_mode = 0;
        float_position = 1;

        gcode_push(c);
        return;
      }
    break;
  }
}