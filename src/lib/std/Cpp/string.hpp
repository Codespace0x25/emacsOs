#pragma once
#include <stddef.h>
#include "../string.h"
#include "../mem.h"


namespace std{
  class string{
  private:
    char* data;
    size_t length;
    void copy_from(const char* str);
  public:
    string();
    string(const char* str);
    string(const string& other);
    ~string();

    size_t size() const;
    const char* c_str() const ;

    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    string operator+(const string& other) const;
    bool operator==(const string& other) const;
  };
}
