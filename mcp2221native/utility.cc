#include <node.h>

const char* ToCString(const v8::String::Utf8Value& value)
{
	return *value ? *value : "<string conversion failed>";
}

long ToByteArray(const v8::Local<v8::Array>& array, unsigned long offset, unsigned long len, unsigned char *p_bin)
{
	unsigned long i;

	if (array->Length() < (offset + len))
		return -1;

	for (i = 0; i < len; i++)
		p_bin[i] = (unsigned char)array->Get(offset + i)->Uint32Value();

	return 0;
}

v8::Local<v8::Array> ToV8ByteArray(v8::Isolate* isolate, const unsigned char *p_bin, unsigned long offset, int len)
{
	v8::Local<v8::Array> array = v8::Array::New(isolate, len);

	for (int i = 0; i < len; i++)
		array->Set(i, v8::Number::New(isolate, p_bin[offset + i]));

	return array;
}

long ToUShortArray(const v8::Local<v8::Array>& array, unsigned long offset, unsigned long len, unsigned short *p_array16)
{
	unsigned long i;

	if (array->Length() < (offset + len))
		return -1;

	for (i = 0; i < len; i++)
		p_array16[i] = (unsigned short)array->Get(offset + i)->Uint32Value();

	return 0;
}

long ToULongArray(const v8::Local<v8::Array>& array, unsigned long offset, unsigned long len, unsigned long *p_array32)
{
	unsigned long i;

	if (array->Length() < (offset + len))
		return -1;

	for (i = 0; i < len; i++)
		p_array32[i] = (unsigned long)array->Get(offset + i)->Uint32Value();

	return 0;
}
