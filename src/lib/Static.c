#include "Static.h"


static volatile char* UserName_ = "System";

char* GetUserName(){
  return (char* )UserName_;
}

void SetUserName(char* data){
  UserName_ = (volatile char*)data;
}
