#ifndef SC6928B_h
#define SC6928B_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TM16XXFonts.h"
#include "Print.h"
//flags for turn on/off
#define ON true
#define OFF false
#define right true
#define left false
//flags for indicating LEDs
#define LED_SE0 0x00
#define LED_SE1 0x01
#define LED_SE2 0x02
#define LED_SE3 0x03
#define LED_SE4 0x04
#define LED_SE5 0x05
#define LED_DVD 0x06
#define LED_VCD 0x07
#define LED_MP3 0x08
#define LED_PLY 0x09
#define LED_PAU 0x0A
#define LED_MP4 0x0B
#define LED_DTS 0x0C
#define LED_DDD 0x0D
#define LED_CL1 0x0E
#define LED_CL2 0x0F
#define LED_1S1 0x10
#define LED_1S2 0x11


class SC6928B : public Print
{
  public:
	// init
	SC6928B(byte _dio_pin, byte _clk_pin, byte _stb_pin);
	
	void begin(boolean active, byte intensity);
	void update();
	void clear();
	void setSeg(byte addr, byte num);
	void setChar(byte _curpos, byte chr);
	void setCursor(byte pos);
	virtual size_t write(byte chr);
	void setTime(int hour, int min, int sec);
	void setLED(byte led);
	void setLEDon(byte led);
	void setLEDoff(byte led);
	void setDisc(boolean onoff);
	void setINT1(int hour);
	void setINT2(int min);
	void setINT3(int sec);
	void setSEGoff(int pos);
	void ticker(String s, boolean dr, int dl);
	byte getButtons();
  protected:
    byte receive();
	void sendData(byte addr, byte data);
    void sendCommand(byte data);
    void send(byte data);
    
    byte _dio_pin;
    byte _clk_pin;
    byte _stb_pin;
	byte _curpos;
//	byte buffer[];
//	byte seg_addr[];
};

#endif
