#include <stdint.h>
#include <stddef.h>
#include "string.h"


size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmp(const char *a, const char *b) {
  while (*a && (*a == *b)) {
    a++;
    b++;
  }
  return *(unsigned char *)a - *(unsigned char *)b;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n--) {
        if (*s1 != *s2 || *s1 == '\0' || *s2 == '\0')
            return (unsigned char)*s1 - (unsigned char)*s2;
        s1++;
        s2++;
    }
    return 0;
}
char *strchr(const char *s, int c) {
    while (*s) {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    return NULL;
}
char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

char* strtok_r(char* str, const char* delim,char** saveptr){
  char *start;
  if(str!=NULL){
    start = str;
  }else{
    start = *saveptr;
  }
  while(*start && strchr(delim,*start)){
    ++start;
  }
  if(*start == '\0'){
    *saveptr = start;
    return NULL;
  }
  char* end = start;
  
  while(*start && !strchr(delim,*start)){
    ++start;
  }
  if(*end){
    *end = '\0';
    *saveptr = end + 1;
  }else{
    *saveptr = end;
  }
  return start;
}

char* strtok(char* str, const char* delim){
  static char* saveptr;
  return strtok_r(str,delim,&saveptr);
}
