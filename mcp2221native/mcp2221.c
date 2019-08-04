#include "libmcp2221.h"
#include "hidapi.h"

#include "mcp2221.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <sys/select.h>
#include <termios.h>
#include <string.h>

static mcp2221_t* myDev;
mcp2221_gpioconfset_t gpioConf;

long MCP2221_initialize(void){
	mcp2221_init();
	int count = mcp2221_find(MCP2221_DEFAULT_VID, MCP2221_DEFAULT_PID, NULL, NULL, NULL);
	if( count < 1 )
		return -1;
		
	myDev = mcp2221_open();
	if(!myDev)
		return -1;

	gpioConf = mcp2221_GPIOConfInit();

	return 0;
}

long Wire_initialize(void){
	mcp2221_i2c_state_t state = MCP2221_I2C_IDLE;
	
	// Stop any transfers
	mcp2221_i2cState(myDev, &state);
	if(state != MCP2221_I2C_IDLE)
		mcp2221_i2cCancel(myDev);

	// Set divider from 12MHz
	mcp2221_i2cDivider(myDev, 26);

	return 0;
}

long Wire_dispose(void){
	mcp2221_exit();
	
	return 0;
}

static unsigned char i2c_address;
static unsigned char i2c_buffer[60];
static unsigned char i2c_count = 0;

void Wire_beginTransmission(unsigned char address){
	mcp2221_i2c_state_t state = MCP2221_I2C_IDLE;
	mcp2221_i2cState(myDev, &state);
	if(state != MCP2221_I2C_IDLE)
		return;
	
	i2c_address = address;
	i2c_count = 0;
}

int Wire_write_byte(unsigned char value){
	i2c_buffer[i2c_count++] = value;
	
	return 1;
}

int Wire_write_str(const char *p_str){
	unsigned char len = strlen(p_str);
	if( (i2c_count + len) > sizeof(i2c_buffer) )
		return -1;

	memmove(&i2c_buffer[i2c_count], p_str, len);
	i2c_count += len;

	return len;
}

int Wire_write_array(const unsigned char *p_data, int length){
	if( ((unsigned long)(i2c_count + length)) > sizeof(i2c_buffer) )
		return -1;

	memmove(&i2c_buffer[i2c_count], p_data, length);
	i2c_count += length;

	return length;
}

void Wire_endTransmission(void){
	mcp2221_i2cWrite(myDev, i2c_address, i2c_buffer, i2c_count, MCP2221_I2CRW_NORMAL);
	i2c_count = 0;

	mcp2221_i2c_state_t state = MCP2221_I2C_IDLE;
	while(1)
	{
		if(mcp2221_i2cState(myDev, &state) != MCP2221_SUCCESS)
			printf("Wire_endTransmission ERROR\n");
		if(state == MCP2221_I2C_IDLE)
			break;
	}
}

int Wire_requestFrom(unsigned char address, int count){
	mcp2221_error ret;
	
	ret = mcp2221_i2cRead(myDev, address, count, MCP2221_I2CRW_NORMAL);
	if( ret != MCP2221_SUCCESS )
		return -1;
	
	i2c_count = count;
	
	return count;
}

int Wire_available(void){
	return i2c_count;
}

unsigned char Wire_read(void){
	if( i2c_count == 0 ){
		printf("Wire_read count over\n");
		return 0xff;
	}

	mcp2221_i2c_state_t state = MCP2221_I2C_IDLE;
	while(1)
	{
		mcp2221_i2cState(myDev, &state);
		if(state == MCP2221_I2C_DATAREADY)
			break;
	}

	unsigned char temp;
	mcp2221_i2cGet(myDev, &temp, 1);
	i2c_count--;
	
	return temp;
}

long GPIO_initialize(void){
	return 0;
}

static mcp2221_gpio_t GPIO_num2pin(unsigned char pin){
	if( pin == 0 )
		return MCP2221_GPIO0;
	else if( pin == 1 )
		return MCP2221_GPIO1;
	else if( pin == 2 )
		return MCP2221_GPIO2;
	else if( pin == 3 )
		return MCP2221_GPIO3;
	
	return -1;
}

void pinMode(unsigned char pin, unsigned char mode){
	if( mode == INPUT ){
		gpioConf.conf[pin].gpios		= GPIO_num2pin(pin);
		gpioConf.conf[pin].mode		= MCP2221_GPIO_MODE_GPIO;
		gpioConf.conf[pin].direction	= MCP2221_GPIO_DIR_INPUT;
	}else if( mode == OUTPUT ){
		gpioConf.conf[pin].gpios		= GPIO_num2pin(pin);
		gpioConf.conf[pin].mode		= MCP2221_GPIO_MODE_GPIO;
		gpioConf.conf[pin].direction	= MCP2221_GPIO_DIR_OUTPUT;
		gpioConf.conf[pin].value		= MCP2221_GPIO_VALUE_HIGH;
	}else if( mode == INPUT_PULLUP ){
		gpioConf.conf[pin].gpios		= GPIO_num2pin(pin);
		gpioConf.conf[pin].mode		= MCP2221_GPIO_MODE_GPIO;
		gpioConf.conf[pin].direction	= MCP2221_GPIO_DIR_INPUT;
		printf("INPUT_PULLUP not supported\n");
	}

	mcp2221_setGPIOConf(myDev, &gpioConf);
//	mcp2221_saveGPIOConf(myDev, &gpioConf);
}

void digitalWrite(unsigned char pin, unsigned char value){
	mcp2221_error res;
	res = mcp2221_setGPIO(myDev, GPIO_num2pin(pin), (value == LOW) ? MCP2221_GPIO_VALUE_LOW : MCP2221_GPIO_VALUE_HIGH );
	if(res != MCP2221_SUCCESS)
		printf("mcp2221_setGPIO error\n");
}

/* 0: invalid, 1: ADC, 2: DAC */
static int analog_initialized[MCP2221_GPIO_COUNT] = { 0 };

static long pinMode_analog(unsigned char pin, unsigned char mode){
	if( mode == INPUT ){
		if( analog_initialized[pin] == 1 )
			return 0;
		
		if( pin != 1 && pin != 2 && pin != 3 )
			return -1;

		gpioConf.conf[pin].gpios = GPIO_num2pin(pin);
		gpioConf.conf[pin].mode = MCP2221_GPIO_MODE_ALT1;
	}else{
		if( analog_initialized[pin] == 2 )
			return 0;
		
		if( pin != 2 && pin != 3 )
			return -1;

		gpioConf.conf[pin].gpios = GPIO_num2pin(pin);
		gpioConf.conf[pin].mode = MCP2221_GPIO_MODE_ALT2;
	}
	

	mcp2221_setGPIOConf(myDev, &gpioConf);
//	mcp2221_saveGPIOConf(myDev, &gpioConf);

	if( mode == INPUT ){
		analog_initialized[pin] = 1;
		mcp2221_setADC(myDev, MCP2221_ADC_REF_VDD);
	}else{
		analog_initialized[pin] = 2;
	}
	
	return 0;
}

static unsigned char read_resolution = 10;
static unsigned char write_resolution = 8;

unsigned short analogRead(unsigned char pin){
	long ret;
	
	ret = pinMode_analog(pin, INPUT);
	if( ret != 0 )
		return 0;
	
	int adc[MCP2221_ADC_COUNT];
	mcp2221_error res;
	res = mcp2221_readADC(myDev, adc);
	if(res != MCP2221_SUCCESS)
		printf("mcp2221_readADC error\n");
	
	return adc[pin - 1] << (read_resolution - 10);
}

void analogWrite(unsigned char pin, unsigned short value){
	long ret;
	
	ret = pinMode_analog(pin, OUTPUT);
	if( ret != 0 )
		return;

	mcp2221_error res;
	res = mcp2221_setDAC(myDev, MCP2221_DAC_REF_VDD , value >> (3 + (write_resolution - 8)));
	if(res != MCP2221_SUCCESS)
		printf("mcp2221_setDAC error\n");
}

void analogReference(unsigned char type){
	if( type != DEFAULT )
		printf("type only DEFAULT" );
}

void analogReadResolution(unsigned char bits){
	read_resolution = bits;
}

void analogWriteResolution(unsigned char bits){
	write_resolution = bits;
}

unsigned char digitalRead(unsigned char pin){
	mcp2221_gpio_value_t values[MCP2221_GPIO_COUNT];
	
	mcp2221_error res;
	res = mcp2221_readGPIO(myDev, values);
	if(res != MCP2221_SUCCESS)
		printf("mcp2221_readGPIO error\n");
	
	return (values[pin] == 0 ) ? LOW : HIGH;
}

static unsigned char serial_port = 0;
static int g_fd = -1;

void Serial_initialize(unsigned char sport){
	serial_port = sport;
}

long Serial_begin(unsigned long speed){
	char port_name[14];
	int res;
    struct termios termios_attr;
	speed_t spd;
	
	sprintf(port_name, "/dev/ttyACM%d", serial_port);
	
	if( speed == 4800 )
		spd = B4800;
	else if( speed == 9600 )
		spd = B9600;
	else if( speed == 19200 )
		spd = B19200;
	else if( speed == 38400 )
		spd = B38400;
	else if( speed == 57600 )
		spd = B57600;
	else if( speed == 115200 )
		spd = B115200;
	else
		return -1;
	
	g_fd = open(port_name, O_RDWR | O_NOCTTY);
	if( g_fd < 0 )
		return -1;

	res = tcgetattr(g_fd, &termios_attr);
    if (res < 0)
		return -1;

	termios_attr.c_iflag &= ~(BRKINT | ICRNL | IGNCR | INLCR | INPCK | PARMRK | ISTRIP | IXOFF | IXON);
    termios_attr.c_iflag |= (IGNBRK | IGNPAR);
    termios_attr.c_oflag &= ~(OPOST | ONLCR | OCRNL | ONOCR | ONLRET);
    termios_attr.c_cflag &= ~(CSTOPB | HUPCL | PARENB);
    termios_attr.c_cflag |= (CLOCAL | CREAD | CS8);
    termios_attr.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON | IEXTEN | ISIG | TOSTOP);
    termios_attr.c_lflag |= NOFLSH;
    termios_attr.c_cc[VMIN] = 0;
    termios_attr.c_cc[VTIME] = 1;

    res = cfsetispeed(&termios_attr, spd);
    if (res < 0)
		return -1;

	res = cfsetospeed(&termios_attr, spd);
    if (res < 0)
		return -1;

    res = tcsetattr(g_fd, TCSANOW, &termios_attr);
    if (res < 0)
		return -1;

	return 0;
}

void Serial_end(void){
	if( g_fd >= 0 ){
		close(g_fd);
		g_fd = -1;
	}
}

static unsigned char serial_buffer[255];
static unsigned short serial_ptr = 0;
static unsigned short serial_index = 0;

int Serial_available(void){
	ssize_t res;

	if( serial_index > 0 ){
		memmove( &serial_buffer[0], &serial_buffer[serial_index], serial_ptr - serial_index);
		serial_ptr -= serial_index;
		serial_index = 0;
	}
	
	res = read(g_fd, &serial_buffer[serial_ptr], sizeof(serial_buffer) - serial_ptr );
	if( res > 0 )
		serial_ptr += res;
	
	return serial_ptr;

}

int Serial_read(void)
{
	if( serial_index == serial_ptr )
		return -1;
	
	return serial_buffer[serial_index++];
}

int Serial_peek(void){
	if( serial_index == serial_ptr )
		return -1;
	
	return serial_buffer[serial_index];
}

void Serial_flush(void){
	serial_index = 0;
	serial_ptr = 0;
}

int Serial_write_array(const unsigned char *p_data, int len){
	ssize_t wrote;

	wrote = write(g_fd, p_data, len );
	if( wrote <= 0 )
		return 0;
	
	return wrote;
}

int Serial_write_byte(unsigned char val){
	return Serial_write_array(&val, 1);
}

int Serial_write_str(const char *p_str){
	return Serial_write_array((unsigned char*)p_str,  strlen(p_str) );
}

int Serial_print(const char *data){
	return Serial_write_str(data);
}

int Serial_println(const char *data){
	int len;
	
	len = Serial_write_str(data);
	len += Serial_write_str("\r\n");
	
	return len;
}
