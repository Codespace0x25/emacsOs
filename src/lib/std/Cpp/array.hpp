#pragma once
#include "../mem.h"
#include "../string.h"
#include <stddef.h>

namespace std {
template <class T, size_t n> class array {
public:
  T& operator[](size_t index){}
  const T& operator[](size_t index) const{}
  T& at(size_t index){}
  const T& at(size_t index) const{}
  constexpr size_t size()const noexcept;
  T* beging(){}
  T* end(){}
  const T* beging()const{
    
  }
  const T* end() const{}
  void fill(const T& valuse){
    
  }



private:
  T data[n];
};
} // namespace std
