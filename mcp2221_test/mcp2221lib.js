const mcp2221 = require('mcp2221native');
var mcp2221_initialized = false;

function mcp2221_initialize(){
  var ret = mcp2221.initialize();
  if( ret != 0 )
    throw 'mcp2221.Serial_initialize error';
    
  mcp2221_initialized = true;
}

class Serial{
  initialize(sport){
    if( !mcp2221_initialized )
      mcp2221_initialize();

    var ret = mcp2221.Serial_initialize(sport);
    if( ret != 0 )
      throw 'mcp2221.Serial_initialize error';

    this.DEC = -1;
    this.BIN = -2;
    this.OCT = -3;
    this.HEX = -4;
  }

  begin(speed){
    return mcp2221.Serial_begin(speed);
  }

  end(){
    mcp2221.Serial_end();
  }

  available(){
    return mcp2221.Serial_available();
  }

  read(){
    return mcp2221.Serial_read();
  }

  peek(){
    return mcp2221.Serial_peek();
  }

  flush(){
    mcp2221.Serial_flush();
  }

  write(param, len){
    if( len === undefined)
      return mcp2221.Serial_write(param);
    else
      return mcp2221.Serial_write(param, len);
  }

  write(buf, len){
  }

  convert(data, format){
    var str;

    if( typeof data == 'string'){
      str = data;
    }else{
      if( format >= 0 ){
        str = data.toFixed(format);
      }else{
        switch(format){
          case this.BIN:
            str = data.toString(2);
            break;
          case this.OCT:
            str = data.toString(8);
            break;
          case this.HEX:
            str = data.toString(16);
            break;
          default:
            str = data.toString(10);
            break;
        }
      }
    }

    return str;
  }

  print(data, format){
    var str = this.convert(data, format);
    return mcp2221.Serial_print(str);
  }

  println(data, format){
    var str = this.convert(data, format);
    return mcp2221.Serial_println(str);
  }
}

class GPIO{
  initialize(){
    if( !mcp2221_initialized )
      mcp2221_initialize();

    var ret = mcp2221.GPIO_initialize();
    if( ret != 0 )
      throw 'mcp2221.GPIO_initialize error';

    this.INPUT = 0;
    this.OUTPUT = 1;

    this.LOW = 0;
    this.HIGH = 1;

    this.DEFAULT = 0;
  }

  pinMode(pin, mode){
    mcp2221.GPIO_pinMode(pin, mode);
  }

  digitalWrite(pin, value){
    mcp2221.GPIO_digitalWrite(pin, value);
  }

  digitalRead(pin){
    return mcp2221.GPIO_digitalRead(pin);
  }

  analogRead(pin){
    return mcp2221.GPIO_analogRead(pin);
  }

  analogWrite(pin, value){
    mcp2221.GPIO_analogWrite(pin, value);
  }

  analogReference(type){
    mcp2221.GPIO_analogReference(type);
  }

  analogReadResolution(bits){
    mcp2221.GPIO_analogReadResolution(bits);
  }
  
  analogWriteResolution(bits){
    mcp2221.GPIO_analogWriteResolution(bits);
  }
}

class Wire{
  initialize(){
    if( !mcp2221_initialized )
      mcp2221_initialize();

    var ret = mcp2221.Wire_initialize();
    if( ret != 0 )
      throw 'mcp2221.Wire_initialize error';
  }

  beginTransmission(address){
    mcp2221.Wire_beginTransmission(address);
  }

  endTransmission(){
    mcp2221.Wire_endTransmission();
  }

  requestFrom(address, count){
    return mcp2221.Wire_requestFrom(address, count);
  }

  available(){
    return mcp2221.Wire_available();
  }

  read(){
    return mcp2221.Wire_read();
  }

  write(param, len){
    if( len === undefined )
      return mcp2221.Wire_write(param);
    else
      return mcp2221.Wire_write(param, len);
  }
}

var gpio = new GPIO();

module.exports = {
  initialize : mcp2221_initialize,
  Serial : new Serial(),
  GPIO: gpio,
  Wire: new Wire(),
  pinMode : gpio.pinMode,
  digitalWrite: gpio.digitalWrite,
  digitalRead: gpio.digitalRead,
  analogRead: gpio.analogRead,
  analogWrite: gpio.analogWrite,
  analogReference: gpio.analogReference,
  analogReadResolution: gpio.analogReadResolution,
  analogWriteResolution: gpio.analogWriteResolution
}