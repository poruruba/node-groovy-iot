const mcp2221 = require('mcp2221native');

const HIGH = 1;
const LOW = 0;

const INPUT = 0;
const OUTPUT = 1;

const GLB_CMDMODE = 0x00;  // Work on 8-bit mode
const GLB_ON = 0xff;  // 8-bit 1 data
const GLB_OFF = 0x00;  // 8-bit 0 data

const MY9221_LED_NUM = 24;

var __pinClock;
var __pinData;
var __greenToRed;
var __led_num;
var __state = new Array(MY9221_LED_NUM);

function Grove_LED_Bar_begin(pinClock, pinData, greenToRed, led_num)
{
	__pinClock = pinClock;
	__pinData = pinData;
	__greenToRed = greenToRed;  // ascending or decending
	__led_num = led_num;

	for (var i = 0; i < __led_num; i++)
		__state[i] = 0x00;  // persist state so individual leds can be toggled

	mcp2221.GPIO_pinMode(__pinClock, OUTPUT);
	mcp2221.GPIO_pinMode(__pinData, OUTPUT);
}


// Send the latch command
async function Grove_LED_Bar_latchData()
{
	mcp2221.GPIO_digitalWrite(__pinData, LOW);
	await wait_async(10);

	for (var i = 0; i < 4; i++)
	{
		mcp2221.GPIO_digitalWrite(__pinData, HIGH);
		mcp2221.GPIO_digitalWrite(__pinData, LOW);
	}
}


// Send 16 bits of data
function Grove_LED_Bar_sendData(data)
{
	var state = 0;
	for (var i = 0; i < 16; i++)
	{
		var state1 = (data & 0x8000) ? HIGH : LOW;
		mcp2221.GPIO_digitalWrite(__pinData, state1);

		//state = digitalRead(__pinClock) ? LOW : HIGH;
		state = 1 - state;
		mcp2221.GPIO_digitalWrite(__pinClock, state);

		data <<= 1;
	}
}


// Change the orientation
// Green to red, or red to green
async function Grove_LED_Bar_setGreenToRed(greenToRed)
{
	__greenToRed = greenToRed;

	await Grove_LED_Bar_setData(__state);
}

function max(a, b){
	return ((a >= b) ? a : b);
}

function min(a, b){
	return ((a <= b) ? a : b);
}

// Set level (0-__led_num)
// Level 0 means all leds off
// Level __led_num means all leds on
// Level 4.5 means 4 LEDs on and the 5th LED's half on
async function Grove_LED_Bar_setLevel(level)
{
	level = max(0.0, min(__led_num, level));
	level *= 8; // there are 8 (noticable) levels of brightness on each segment

	// Place number of 'level' of 1-bits on __state
	for (var i = 0; i < __led_num; i++) {
		__state[i] = (level > 8) ? ~0 : (level > 0) ? ~(~0 << Math.floor(level)) : 0;

		level -= 8;
	};

	await Grove_LED_Bar_setData(__state);
}


// Set a single led
// led (1-__led_num)
// brightness (0-1)
async function Grove_LED_Bar_setLed(led, brightness)
{
	led = max(1, min(__led_num, led));
	brightness = max(0.0, min(brightness, 1.0));

	// Zero based index 0-9 for bitwise operations
	led--;

	// 8 (noticable) levels of brightness
	// 00000000 darkest
	// 00000011 brighter
	// ........
	// 11111111 brightest
	__state[led] = ~(~0 << Math.floor(brightness*8));

	await Grove_LED_Bar_setData(__state);
}


// Toggle a single led
// led (1-__led_num)
async function Grove_LED_Bar_toggleLed(led)
{
	led = max(1, min(__led_num, led));

	// Zero based index 0-9 for bitwise operations
	led--;

	__state[led] = __state[led] ? 0 : ~0;

	await Grove_LED_Bar_setData(__state);
}

// each element in the state will hold the brightness level
// 00000000 darkest
// 00000011 brighter
// ........
// 11111111 brightest
async function Grove_LED_Bar_setData(state)
{
	Grove_LED_Bar_sendData(GLB_CMDMODE);

	for (var i = 0; i < __led_num; i++)
	{
		if (__greenToRed)
		{
			// Go backward on state
			Grove_LED_Bar_sendData(state[__led_num - i - 1]);
		}
		else
		{
			// Go forward on state
			Grove_LED_Bar_sendData(state[i]);
		}
	}

	// Two extra empty bits for padding the command to the correct length
	Grove_LED_Bar_sendData(0x00);
	Grove_LED_Bar_sendData(0x00);

	await Grove_LED_Bar_latchData();
}

async function Grove_LED_Bar_setBits(bits)
{
	for (var i = 0; i < __led_num; i++)
	{
		if ((bits % 2) == 1)
			__state[i] = 0xFF;
		else
			__state[i] = 0x00;
		bits >>= 1;
	}

	await Grove_LED_Bar_setData(__state);
}


// Return the current bits
function Grove_LED_Bar_getBits()
{
	var __bits = 0x00;
	for (var i = 0; i < __led_num; i++)
	{
		if (__state[i] != 0x0)
			__bits |= (0x1 << i);
	}
	return __bits;
}

function Grove_LED_Bar_setLedNum(num){
    __led_num = num;
}

function wait_async(timeout){
	return new Promise((resolve, reject) =>{
		setTimeout(resolve, timeout);
	});
}

async function test(){
	var ret = mcp2221.initialize();
	console.log(ret);

	ret = mcp2221.GPIO_initialize();
	console.log(ret);
	
	Grove_LED_Bar_begin(1, 0, 0, 10);
	
	await Grove_LED_Bar_setBits(0x0ff);
	
	await Grove_LED_Bar_toggleLed(5);

	ret = mcp2221.Serial_begin(115200);
	console.log(ret);

	ret = mcp2221.Serial_println('Hello World\n');
	console.log(ret);

}

test();
