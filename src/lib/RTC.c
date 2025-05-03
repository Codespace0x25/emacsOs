#include "DataPort.h"
#include "Screen.h"
#include <stdint.h>
#include "RTC.h"

#define RTC_INDEX_PORT 0x70
#define RTC_DATA_PORT  (RTC_INDEX_PORT +1)

#define RTC_SECONDS    0x00
#define RTC_MINUTES    0x02
#define RTC_HOURS      0x04
#define RTC_DAY        0x07
#define RTC_MONTH      0x08
#define RTC_YEAR       0x09
#define RTC_REG_A      0x0A
#define RTC_REG_B      0x0B
#define RTC_REG_C      0x0C
#define RTC_REG_D      0x0D


void rtc_wait_for_update(){
  outb(RTC_INDEX_PORT, RTC_REG_A);
  while (inb(RTC_DATA_PORT)& 0x80);
}

uint8_t rtc_read(uint8_t reg){
  outb(RTC_INDEX_PORT, reg);
  return inb(RTC_DATA_PORT);
}

void rtc_get_time(uint8_t *hour, uint8_t *minut, uint8_t *second){
  rtc_wait_for_update();
  *second= rtc_read(RTC_SECONDS);
  *minut = rtc_read(RTC_MINUTES);
  *hour  = rtc_read(RTC_HOURS);
  uint8_t statuse_b;
  outb(RTC_INDEX_PORT, RTC_REG_B);
  statuse_b = inb(RTC_DATA_PORT);
  
  if(!(statuse_b&0x04)){
    *second=(*second & 0xF) +((*second / 16)* 10) ;
    *minut = (*minut & 0xF) +((*minut/ 16)* 10);
    *hour  =  (*hour & 0xF) +((*hour / 16)* 10);
  }
}

void rtc_enable_periodic_interrupt(){
  outb(RTC_INDEX_PORT, RTC_REG_B);
  uint8_t prev = inb(RTC_DATA_PORT);
  outb(RTC_INDEX_PORT, RTC_REG_B);
  outb(RTC_DATA_PORT,prev | 0x40);
}
