#include <node.h>

#include "mcp2221.h"
#include "utility.h"

namespace mcp2221native {

	void mcp2221_initialize(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 1 || !args[0]->IsNumber())
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_initialize")));
			return;
		}

		unsigned char sport = (unsigned char)args[0]->Uint32Value();

		ret = MCP2221_initialize(sport);
		if (ret != 0)
		{
			isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "MCP2221_initialize")));
			return;
		}

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Wire_initialize(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_initialize")));
			return;
		}

		ret = Wire_initialize();
		if (ret != 0)
		{
			isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Wire_initialize")));
			return;
		}

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Wire_dispose(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_dispose")));
			return;
		}

		ret = Wire_dispose();
		if (ret != 0)
		{
			isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Wire_dispose")));
			return;
		}

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Wire_beginTransmission(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		if( args.Length() != 1 || !args[0]->IsNumber())
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_beginTransmission")));
			return;
		}

		unsigned char address = (unsigned char)args[0]->Uint32Value();

		Wire_beginTransmission(address);

		args.GetReturnValue().Set(v8::Number::New(isolate, 0));
	}

	void mcp2221_Wire_write(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;
		v8::Local<v8::Array> input;

		if( !((args.Length() == 1 && (args[0]->IsNumber() || args[0]->IsString())) || (args.Length() == 2 && args[0]->IsArray() && args[1]->IsNumber())) )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_write")));
			return;
		}

		if( args.Length() == 1 && args[0]->IsNumber() ){
			unsigned char value = (unsigned char)args[0]->Uint32Value();
			ret = Wire_write_byte(value);
		}else if( args.Length() == 1 && args[0]->IsString() ){
			const char *p_str = ToCString(v8::String::Utf8Value(args[0]));
			ret = Wire_write_str(p_str);
		}else if( args.Length() == 2 && args[0]->IsArray() && args[1]->IsNumber() ){
			int length = args[1]->Uint32Value();
			input = v8::Local<v8::Array>::Cast(args[0]);
			unsigned char data[255];
			ret = ToByteArray(input, 0, length, data);
			if (ret != 0)
			{
				isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "ToByteArray")));
				return;
			}
			ret = Wire_write_array(data, length);
		}else{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_write")));
			return;
		}

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Wire_endTransmission(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		if( args.Length() != 0)
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_beginTransmission")));
			return;
		}

		Wire_endTransmission();

		args.GetReturnValue().Set(v8::Number::New(isolate, 0));
	}
	
	void mcp2221_Wire_requestFrom(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 2 || !args[0]->IsNumber() || !args[1]->IsNumber())
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_requestFrom")));
			return;
		}

		unsigned char address = (unsigned char)args[0]->Uint32Value();
		int count = args[1]->Uint32Value();

		ret = Wire_requestFrom(address, count);

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Wire_available(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0)
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_available")));
			return;
		}

		ret = Wire_available();

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}
	
	void mcp2221_Wire_read(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0)
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Wire_read")));
			return;
		}

		ret = Wire_read();

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_GPIO_initialize(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_GPIO_initialize")));
			return;
		}

		ret = GPIO_initialize();
		if (ret != 0)
		{
			isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "GPIO_initialize")));
			return;
		}

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_GPIO_pinMode(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		if( args.Length() != 2 || !args[0]->IsNumber() || !args[1]->IsNumber() )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_GPIO_pinMode")));
			return;
		}
		
		unsigned char pin = (unsigned char)args[0]->Uint32Value();
		unsigned char mode = (unsigned char)args[1]->Uint32Value();

		pinMode(pin, mode);

		args.GetReturnValue().Set(v8::Number::New(isolate, 0));
	}

	void mcp2221_GPIO_digitalWrite(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		if( args.Length() != 2 || !args[0]->IsNumber() || !args[1]->IsNumber() )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_GPIO_digitalWrite")));
			return;
		}
		
		unsigned char pin = (unsigned char)args[0]->Uint32Value();
		unsigned char value = (unsigned char)args[1]->Uint32Value();

		digitalWrite(pin, value);

		args.GetReturnValue().Set(v8::Number::New(isolate, 0));
	}

	void mcp2221_GPIO_digitalRead(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 1 || !args[0]->IsNumber() )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_GPIO_digitalRead")));
			return;
		}
		
		unsigned char pin = (unsigned char)args[0]->Uint32Value();

		ret = digitalRead(pin);

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Serial_begin(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 1 || !args[0]->IsNumber() )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_begin")));
			return;
		}
		
		unsigned long speed = args[0]->Uint32Value();

		ret = Serial_begin(speed);
		if (ret != 0)
		{
			isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Serial_begin")));
			return;
		}

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Serial_end(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_end")));
			return;
		}
		
		Serial_end();

		args.GetReturnValue().Set(v8::Number::New(isolate, 0));
	}

	void mcp2221_Serial_available(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_available")));
			return;
		}
		
		ret = Serial_available();

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Serial_read(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_read")));
			return;
		}
		
		ret = Serial_read();

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Serial_peek(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_peek")));
			return;
		}
		
		ret = Serial_peek();

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Serial_flush(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();

		if( args.Length() != 0 )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_flush")));
			return;
		}
		
		Serial_flush();

		args.GetReturnValue().Set(v8::Number::New(isolate, 0));
	}
	
	void mcp2221_Serial_write(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;
		v8::Local<v8::Array> input;

		if( !((args.Length() == 1 && (args[0]->IsNumber() || args[0]->IsString())) || (args.Length() == 2 && args[0]->IsArray() && args[1]->IsNumber())) )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_write")));
			return;
		}

		if( args.Length() == 1 && args[0]->IsNumber() ){
			unsigned char value = (unsigned char)args[0]->Uint32Value();
			ret = Serial_write_byte(value);
		}else if( args.Length() == 1 && args[0]->IsString() ){
			const char *p_str = ToCString(v8::String::Utf8Value(args[0]));
			ret = Serial_write_str(p_str);
		}else if( args.Length() == 2 && args[0]->IsArray() && args[1]->IsNumber() ){
			int length = args[1]->Uint32Value();
			input = v8::Local<v8::Array>::Cast(args[0]);
			unsigned char data[255];
			ret = ToByteArray(input, 0, length, data);
			if (ret != 0)
			{
				isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "ToByteArray")));
				return;
			}
			ret = Serial_write_array(data, length);
		}else{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_write")));
			return;
		}

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Serial_print(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 1 || !args[0]->IsString() )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_print")));
			return;
		}
		
		const char *p_str = ToCString(v8::String::Utf8Value(args[0]));

		ret = Serial_print(p_str);

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}

	void mcp2221_Serial_println(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		long ret;

		if( args.Length() != 1 || !args[0]->IsString() )
		{
			isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "mcp2221_Serial_println")));
			return;
		}
		
		const char *p_str = ToCString(v8::String::Utf8Value(args[0]));

		ret = Serial_println(p_str);

		args.GetReturnValue().Set(v8::Number::New(isolate, ret));
	}
	
	void init(v8::Local<v8::Object> exports)
	{
		NODE_SET_METHOD(exports, "initialize", mcp2221_initialize);

		NODE_SET_METHOD(exports, "Wire_initialize", mcp2221_Wire_initialize);
		NODE_SET_METHOD(exports, "Wire_dispose", mcp2221_Wire_dispose);
		NODE_SET_METHOD(exports, "Wire_beginTransmission", mcp2221_Wire_beginTransmission);
		NODE_SET_METHOD(exports, "Wire_write", mcp2221_Wire_write);
		NODE_SET_METHOD(exports, "Wire_endTransmission", mcp2221_Wire_endTransmission);
		NODE_SET_METHOD(exports, "Wire_requestFrom", mcp2221_Wire_requestFrom);
		NODE_SET_METHOD(exports, "Wire_available", mcp2221_Wire_available);
		NODE_SET_METHOD(exports, "Wire_read", mcp2221_Wire_read);

		NODE_SET_METHOD(exports, "GPIO_initialize", mcp2221_GPIO_initialize);
		NODE_SET_METHOD(exports, "GPIO_pinMode", mcp2221_GPIO_pinMode);
		NODE_SET_METHOD(exports, "GPIO_digitalWrite", mcp2221_GPIO_digitalWrite);
		NODE_SET_METHOD(exports, "GPIO_digitalRead", mcp2221_GPIO_digitalRead);

		NODE_SET_METHOD(exports, "Serial_begin", mcp2221_Serial_begin);
		NODE_SET_METHOD(exports, "Serial_end", mcp2221_Serial_end);
		NODE_SET_METHOD(exports, "Serial_available", mcp2221_Serial_available);
		NODE_SET_METHOD(exports, "Serial_read", mcp2221_Serial_read);
		NODE_SET_METHOD(exports, "Serial_peek", mcp2221_Serial_peek);
		NODE_SET_METHOD(exports, "Serial_flush", mcp2221_Serial_flush);
		NODE_SET_METHOD(exports, "Serial_write", mcp2221_Serial_write);
		NODE_SET_METHOD(exports, "Serial_print", mcp2221_Serial_print);
		NODE_SET_METHOD(exports, "Serial_println", mcp2221_Serial_println);
	}

	NODE_MODULE(addon, init)
}
