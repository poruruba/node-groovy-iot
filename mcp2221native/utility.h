#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <node.h>

const char* ToCString(const v8::String::Utf8Value& value);
long ToByteArray(const v8::Local<v8::Array>& array, unsigned long offset, unsigned long len, unsigned char *p_bin);
v8::Local<v8::Array> ToV8ByteArray(v8::Isolate* isolate, const unsigned char *p_bin, unsigned long offset, int len);
long ToUShortArray(const v8::Local<v8::Array>& array, unsigned long offset, unsigned long len, unsigned short *p_array16);
long ToULongArray(const v8::Local<v8::Array>& array, unsigned long offset, unsigned long len, unsigned long *p_array32);

#endif
