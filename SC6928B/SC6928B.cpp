
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SC6928B.h"
byte _curpos = 0x00;
byte buffer[14] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00, 0x00};
const byte seg_addr[6]={0x00,0x07,0x01,0x01,0x02,0x03};//no bit of digital segments
//                       SG0  SG1  SG2  SG3  SG4  SG5  DVD  VCD  MP3  PLY  PAU  MP4  DTS  DDD  CL1  CL2  1S1  1S2   //name   -|
const byte led_addr[18]={0x0C,0x0A,0x02,0x04,0x08,0x06,0x00,0x06,0x02,0x00,0x06,0x04,0x0A,0x02,0x08,0x08,0x0A,0x04};//adress -| for the signs's and disc's leds
const byte led_val[18]= {0x40,0x40,0x40,0x40,0x40,0x40,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x10,0x20,0x20};//byte   -|

SC6928B::SC6928B(byte _dio_pin, byte _clk_pin, byte _stb_pin) {
  this->_dio_pin = _dio_pin;
  this->_clk_pin = _clk_pin;
  this->_stb_pin = _stb_pin;

  pinMode(_dio_pin, OUTPUT);
  pinMode(_clk_pin, OUTPUT);
  pinMode(_stb_pin, OUTPUT);

  digitalWrite(_stb_pin, HIGH);
  digitalWrite(_clk_pin, HIGH);

  sendCommand(0x40);
  sendCommand(0x80);

  digitalWrite(_stb_pin, LOW);
  send(0xC0);
  clear();
  digitalWrite(_stb_pin, HIGH);
}

void SC6928B::begin(boolean active = true, byte intensity = 1) {
  sendCommand(0x80 | (active ? 8 : 0) | min(7, intensity));
}

void SC6928B::update() {
  for (int i=0; i<14; i++)
    sendData(i, buffer[i]);
}

void SC6928B::setTime(int hour,int min,int sec) {
  if(hour>199){
    setChar(0, '-');
    setChar(1, '-');
  }else if(hour>99){
    setLEDon(16);
    setLEDon(17);
    setSeg(0, (hour%1000)%10);
	  setSeg(1, (hour%100)%10);
  }else{
    setSeg(0, (hour/10));
	  setSeg(1, (hour%100)%10);
  }
  if(min>99){
    setChar(0, '-');
    setChar(1, '-');
  }else{
    setSeg(2, (min/10));
	  setSeg(3, (min%10));
  }
	if(sec>99){
    setChar(0, '-');
    setChar(1, '-');
  }else{
	  setSeg(4, (sec/10));
	  setSeg(5, (sec%10));
  }
	update();
}

void SC6928B::setINT1(int hour){
  if(hour>199){
    setChar(0, '-');
    setChar(1, '-');
  }else if(hour>99){
    setLEDon(16);
    setLEDon(17);
    setSeg(0, (hour%1000)%10);
	  setSeg(1, (hour%100)%10);
  }else{
    setSeg(0, (hour/10));
	  setSeg(1, (hour%100)%10);
  }
  update();
}
void SC6928B::setINT2(int min){
  if(min>99){
    setChar(2, '-');
    setChar(3, '-');
  }else{
    setSeg(2, (min/10));
	  setSeg(3, (min%10));
  }
  update();
}
void SC6928B::setINT3(int sec){
	if(sec>99){
    setChar(4, '-');
    setChar(5, '-');
  }else{
	  setSeg(4, (sec/10));
	  setSeg(5, (sec%10));
  }
  update();
}

void SC6928B::clear() {
	for (int i=0; i<14; i++)
		buffer[i]=0x00;
	_curpos=0x00;
	update();
}

void SC6928B::setLED(byte led){
	buffer[led_addr[led]] = buffer[led_addr[led]] ^ led_val[led];
	update();
}

void SC6928B::setLEDon(byte led){
	buffer[led_addr[led]] |= led_val[led];//= buffer[led_addr[led]] | led_val[led];
	update();
}

void SC6928B::setLEDoff(byte led){
	buffer[led_addr[led]] &= 0xFF - led_val[led];//= buffer[led_addr[led]] & (0xFF - led_val[led]);
	update();
}

void SC6928B::setDisc(boolean onoff){
	if (onoff) {
		for(int i=0;i<6;i++)// turn on all disc segments
			buffer[led_addr[i]] = buffer[led_addr[i]] | led_val[i];
	} else {
		for(int i=0;i<6;i++)
			buffer[led_addr[i]] = buffer[led_addr[i]] & (0xFF - led_val[i]);
	}
	update();
}

void SC6928B::ticker(String s, boolean dr, int dl){
  if (dr){
    int j;
    int k=0;
    for(int i=0; i<s.length(); i++){
      for(j=s.length()-1; j>=j-i; j--){
        if(k+i==-1){
          break;
        }
        if(k+i<6){
          setChar(k+i, s[j]);
        }
        k--;
      }
    k=0;
    delay(dl);
    if(i!=s.length()-1){
      setSEGoff(i);
    }
    }
    for(int i=1; i<7; i++){
      for(j=s.length()-1; j>=0; j--){
        if(j+i>5){
          continue;
        }
        setChar(j+i, s[j]);
      }
    setSEGoff(i-1);
    delay(dl);
    }
  }else{
    for(int i=7; i>=0; i--){
      for(int j=0; j<=s.length()-1; j++){
        if(j+i>5){
          continue;
        }
        setChar(j+i, s[j]);
      }
    setSEGoff(i-1);
    if(i!=0){
      delay(dl);
    }  
    }
    int j;
    int k=0;
    setSEGoff(5);
    for(int i=s.length()-1; i>=0; i--){
      for(j=s.length()-1; j>=0; j--){
        if(k+i==-1){
          break;
        }
        if(k+i<6){
          setChar(k+i, s[j]);
        }
        k--;
      }
    k=0;
    delay(dl); 
    setSEGoff(i);
    }
  }
}

byte SC6928B::getButtons() {
  byte keys = 0;

  digitalWrite(_stb_pin, LOW);
  send(0x42);
  for (int i = 0; i < 5; i++) {
    keys |= receive() << i;
  }
  digitalWrite(_stb_pin, HIGH);

  return keys;
}

size_t SC6928B::write(byte chr){
	if(_curpos<0x07) {
		setChar(_curpos, chr);
		_curpos++;
	}
}

void SC6928B::setCursor(byte pos){
	_curpos = pos;
}
/*********** mid level  **********/
void SC6928B::sendData(byte addr, byte data) {
  sendCommand(0x44);
  digitalWrite(_stb_pin, LOW);
  send(0xC0 | addr);
  send(data);
  digitalWrite(_stb_pin, HIGH);
}

void SC6928B::sendCommand(byte data) {
  digitalWrite(_stb_pin, LOW);
  send(data);
  digitalWrite(_stb_pin, HIGH);
}

void SC6928B::setSeg(byte addr, byte num) {
  int b=0;
  if((addr==0)||(addr==2)){
    b=1;
  }else{
    b=0;
  }
  for(int i=0; i<7; i++){
      bitWrite(buffer[(i*2)+b], seg_addr[addr], bitRead(NUMBER_FONT[num],i));
    }
  update();
}

void SC6928B::setSEGoff(int pos){
  int b=0;
  if(pos<6){
    if((pos==0)||(pos==2)){
      b=1;
    }else{
      b=0;
    }
    for(int i=0; i<7; i++){
        bitWrite(buffer[(i*2)+b], seg_addr[pos], 0x00);
      }
    update();
  }
}

void SC6928B::setChar(byte addr, byte chr) {
  int b=0;
  if((addr==0)||(addr==2)){
    b=1;
  }else{
    b=0;
  }
  for(int i=0; i<7; i++){
      bitWrite(buffer[(i*2)+b], seg_addr[addr], bitRead(FONT_DEFAULT[chr - 0x20],i));
    }
	update();
}
/************ low level **********/
void SC6928B::send(byte data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(_clk_pin, LOW);
    digitalWrite(_dio_pin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(_clk_pin, HIGH);
  }
}

byte SC6928B::receive() {
  byte temp = 0;

  // Pull-up on
  pinMode(_dio_pin, INPUT);
  digitalWrite(_dio_pin, HIGH);

  for (int i = 0; i < 8; i++) {
    temp >>= 1;

    digitalWrite(_clk_pin, LOW);

    if (digitalRead(_dio_pin)) {
      temp |= 0x80;
    }

    digitalWrite(_clk_pin, HIGH);
  }

  // Pull-up off
  pinMode(_dio_pin, OUTPUT);
  digitalWrite(_dio_pin, LOW);

  return temp;
}
