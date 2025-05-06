#include "Static.h"
#include <stdint.h>


volatile static  int8_t* UserName_ = "System";

char* GetUserName(){
  return (char*)UserName_;
}

void SetUserName(char* data){
  UserName_ = data;
}
