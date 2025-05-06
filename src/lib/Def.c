#include "Allocator.h"
#include "Com1.h"
#include "DataPort.h"
#include "GDT.h"
#include "Music.h"
#include "PC_Speaker.h"
#include "Song.h"
#include "SoundBlaster.h"
#include "PIT.h"
#include "Page.h"
#include "RTC.h"
#include "Screen.h"
#include "PCI.h"
#include "Static.h"
#include "idt.h"
#include "std/string.h"
#include <stdint.h>

void putPS1(){
  putS(GetUserName());
  putS("> ");
}
