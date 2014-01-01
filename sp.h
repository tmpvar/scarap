#ifndef __SP
#define __SP

void sp_init();
void sp_write(char c);
void sp_read_callback(void (*cb)(char));

#endif