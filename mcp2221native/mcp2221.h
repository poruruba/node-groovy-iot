#ifndef _MCP2221_H_
#define _McP2221_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

long MCP2221_initialize(void);

long Wire_initialize(void);
long Wire_dispose(void);
void Wire_beginTransmission(unsigned char address);
long Wire_write_byte(unsigned char value);
long Wire_write_str(const char *p_str);
long Wire_write_array(const unsigned char *p_data, unsigned char length);
void Wire_endTransmission(void);
long Wire_requestFrom(unsigned char address, unsigned char count);
unsigned char Wire_available(void);
unsigned char Wire_read(void);

#define HIGH	1
#define LOW		0

#define INPUT			0
#define OUTPUT			1
#define INPUT_PULLUP	2

long GPIO_initialize(void);
void pinMode(unsigned char pin, unsigned char mode);
void digitalWrite(unsigned char pin, unsigned char value);
unsigned char digitalRead(unsigned char pin);

long Serial_begin(unsigned long speed);
void Serial_end(void);
int Serial_available(void);
int Serial_read(void);
int Serial_peek(void);
void Serial_flush(void);
int Serial_write_byte(unsigned char val);
int Serial_write_str(const char *p_str);
int Serial_write_array(const unsigned char *p_str, int len);
int Serial_print(const char *data);
int Serial_println(const char *data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
