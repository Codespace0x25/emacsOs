#include "DataPort.h"
#include "Screen.h"
#include <stdint.h>


void rtc_wait_for_update();

uint8_t rtc_read(uint8_t reg);

void rtc_get_time(uint8_t *hour, uint8_t *minut, uint8_t *second);

void rtc_enable_periodic_interrupt();
