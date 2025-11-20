# SC6928B  
7-Segments indicator Arduino Library  
  
Example  
  
#include <SC6928B.h>  
// define - data pin D8, clock pin D8 and strobe pin D10  
// обьявление портов: DIO - порт D8, CLK - D9, STB - D10  
SC6928B dvdLED(8, 9, 10);  

void setup() {  
  dvdLED.begin(ON, 8);  
  dvdLED.clear();  
}   
void loop(){  
  //dvdLED.getButtons(); //Get the button click ID. I have it 8, 32, 64  
  dvdLED.print("HELLO");  
  delay(1000);  
  dvdLED.clear(); //Clear display.  
  dvdLED.setTime(188, 88, 99); //Show three numbers between colon.
  dvdLED.setLEDon(LED_CL1); //Turn on the LED  
  dvdLED.setLEDon(LED_CL2); //Turn on the LED 
  delay(1000);  
  dvdLED.clear();   
  delay(1000);  
  dvdLED.setINT1(199); //Show the number in the first department. Maximum 199  
  delay(1000);  
  dvdLED.setLEDoff(LED_1S1); //Disabling number 1.  
  dvdLED.setLEDoff(LED_1S2); //Disabling number 1.  
  dvdLED.setSEGoff(0); //Disable segment by number.  
  dvdLED.setSEGoff(1); //Disable segment by number  
  delay(1000);  
  dvdLED.setINT2(99); //Show the number in the second department. Maximum 99  
  delay(1000);  
  dvdLED.setSEGoff(2); //Disable segment by number  
  dvdLED.setSEGoff(3); //Disable segment by number  
  delay(1000);  
  dvdLED.setINT3(99); //Show the number in the third department. Maximum 99  
  delay(1000);  
  dvdLED.setSEGoff(4); //Disable segment by number  
  dvdLED.setSEGoff(5); //Disable segment by number  
  delay(1000);  
  dvdLED.ticker("Goodbye", left, 300);  //Running line, right to left, delay (300) between each character  
  dvdLED.ticker("Goodbye", right, 300); //Running line, left to right, delay (300) between each character  
}  
  
/* sequence of LEDs:  
   порядок светодиодов:  
        LED_SE0 0x00  
        LED_SE1 0x01  
        LED_SE2 0x02  
        LED_SE3 0x03  
        LED_SE4 0x04  
        LED_SE5 0x05  
        LED_DVD 0x06  
        LED_VCD 0x07  
        LED_MP3 0x08  
        LED_PLY 0x09  
        LED_PAU 0x0A  
        LED_MP4 0x0B  
        LED_DTS 0x0C  
        LED_DDD 0x0D  
        LED_CL1 0x0E  
        LED_CL2 0x0F  
        LED_1S1 0x10  
        LED_1S2 0x11  
*/  

