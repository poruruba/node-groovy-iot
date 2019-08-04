const { Serial, GPIO, Wire, pinMode, digitalRead, analogRead } = require('./mcp2221lib');
const ledbar = require('./Grove_LED_Bar');
const TSL2561 = require('./Digital_Light_TSL2561');

async function test(){
	Serial.initialize(0);
	GPIO.initialize();
	Wire.initialize();

	Serial.begin(9600);

	var ret = Serial.println("Hello World");
	console.log(ret);

/* Grove - PIR Motion Sensor */
/*	
	pinMode(2, GPIO.INPUT);
	while(true){
		if( digitalRead(2) )
			console.log("Hi, people is comming");
	}
*/

/* Grove - Digital Light Sensor */
/*
	TSL2561.init();
	console.log(await TSL2561.readVisibleLux());
*/

/* Grove - LED Bar */
/*
	var Grove_LED_Bar = new ledbar(1, 0, 0, 10);
	Grove_LED_Bar.setLevel(3);
*/

/* Grove - Light Sensor(P) */
	console.log( analogRead(2) );

	return;
}

test();
