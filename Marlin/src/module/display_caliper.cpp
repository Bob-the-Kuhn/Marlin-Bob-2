/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "../inc/MarlinConfig.h"


#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

#include "display_caliper.h"
#include "../MarlinCore.h"

#if HAS_STATUS_MESSAGE

//#include "../gcode.h"
#include "../lcd/marlinui.h"


//void display_caliper(void)
//{
//  static uint16_t temp_counter;
//
//  char tmp_1[32];
//  sprintf_P(tmp_1, PSTR("Bob:%d"), temp_counter++);
//  ui.set_status(tmp_1, true);
//
//}


//
// based on code fro https://www.instructables.com/DIY-Digital-Readout-for-Drill-Press/
//

void display_caliper_setup()
{

  pinMode(CALIPER_DATA_PIN , INPUT);
  pinMode(CALIPER_CLOCK_PIN, INPUT);

  pinMode(PD5, OUTPUT);
  pinMode(PD6, OUTPUT);

}


char buf[20];
char buf2[10];
millis_t tmpTime;
int sign;
int inches_caliper;
long value;
float result;
bool mm = true; //define mm to false if you want inches values
void readCaliper();

void display_caliper(void)
{


  readCaliper();
  buf[0]=' ';

  if(mm)
    {
      //if(inches)
      //  {
      //
      //    dtostrf(float(result*25.4f),6,3,buf+1); strcat(buf," mm ");
      //   //char tmp_1[32];
      //   //sprintf_P(tmp_1, PSTR("%.1fmx"), result*25.4);
      //   //ui.set_status(tmp_1, true);
      //
      //  }
      // else
      //   {
//           dtostrf(result,6,2,buf+1); strcat(buf," mm ");
      dtostrf(result,6,2,buf2+1); strcat(buf2," mm ");
      sprintf_P(buf, PSTR("%s %4li %i"), buf2, value, inches_caliper);
      //   }
    }
    else
    {
     // if(inches)
     //   {
          dtostrf(result,6,3,buf+1); strcat(buf," in ");
     //   }
     //  else
     //    {
     //      dtostrf(float(result/25.4f),6,3,buf+1); strcat(buf," in ");
     //    }
    }
  ui.set_status(buf, true);
}

void delay_us(unsigned long delay_uSec) {
  unsigned long start_delay = millis();
  unsigned long end_delay = start_delay + delay_uSec;
  while (millis() < end_delay) {};

}

void readCaliper()  // always return the result in the desired units (per mm setting)
                    //  0.01mm/0.0005" indicator
{
  #define CALIPER_READ_TIMEOUT 10  // maximum time (mS)  allowed to read a 24 bit packet
  sign=1;
  value=0;
  inches_caliper=0;
  float temp_div = 0 ;

  tmpTime=millis();
    digitalWrite(PD6, HIGH);        // triple pulse to show looking for new packet
    delay(4);
    digitalWrite(PD6, LOW);
    delay(4);
    digitalWrite(PD6, HIGH);
    delay(4);
    digitalWrite(PD6, LOW);
    delay(4);
    digitalWrite(PD6, HIGH);
    delay(4);
    digitalWrite(PD6, LOW);
  while((millis()-tmpTime)<CALIPER_READ_TIMEOUT) {digitalWrite(PD6, !digitalRead(PD6)); if (digitalRead(CALIPER_CLOCK_PIN)==LOW) {tmpTime=millis();}} // wait for inter-packet quiet time
  //while((millis()-tmpTime)<CALIPER_READ_TIMEOUT) { if (digitalRead(CALIPER_CLOCK_PIN)==LOW) {tmpTime=millis();}digitalWrite(PD5, !digitalRead(PD5));
  //digitalWrite(PD6, digitalRead(CALIPER_DATA_PIN));} // wait for inter-packet quiet time

  while(digitalRead(CALIPER_CLOCK_PIN)==HIGH) {digitalWrite(PD5, !digitalRead(PD5));}  ;  // wait for start of packet
  //while(digitalRead(CALIPER_CLOCK_PIN)==HIGH) {
  //digitalWrite(PD6, !digitalRead(PD6));}  ;  // wait for start of packet
  // first clock received - sample data on rising edge of clock
  digitalWrite(PD5, LOW);
  digitalWrite(PD6, LOW);
  tmpTime=millis();
  for(int i=0;i<24;i++) {
    digitalWrite(PD5, HIGH);  //single pulse to show when reading bit
    //digitalWrite(PD5, digitalRead(CALIPER_CLOCK_PIN));
    //digitalWrite(PD6, digitalRead(CALIPER_DATA_PIN));
//    while(digitalRead(CALIPER_CLOCK_PIN)==HIGH) {} // wait for high to low transition on clock
   // while(digitalRead((CALIPER_CLOCK_PIN)==LOW) && ((millis()-tmpTime)<CALIPER_READ_TIMEOUT) ) {}  // wait for low to high transition on clock
    while(digitalRead((CALIPER_CLOCK_PIN)==LOW)) {}  // wait for low to high transition on clock
//    while(digitalRead(CALIPER_CLOCK_PIN)==LOW) {}  // wait for low to high transition on clock
//    while(digitalRead(CALIPER_CLOCK_PIN)==HIGH) {} // wait for high to low transition on clock
    //digitalWrite(PD5, HIGH);  //single pulse to show when reading bit


    if(digitalRead(CALIPER_DATA_PIN)==HIGH) {
      if(i<20) value|=(1<<i);
      if(i==20) sign=-1;
      if(i==23) {inches_caliper=2;}
    }
    if(i==0) digitalWrite(PD6, HIGH);  // single pulse to show reading first bit
    if(i==23) {
      digitalWrite(PD6, HIGH);        // double pulse to show reading 23rd bit
      digitalWrite(PD6, !digitalRead(PD6));
      digitalWrite(PD6, !digitalRead(PD6));
    }
    digitalWrite(PD6, LOW);
    digitalWrite(PD5, LOW);
    //while(digitalRead((CALIPER_CLOCK_PIN)==HIGH) && ((millis()-tmpTime)<CALIPER_READ_TIMEOUT) ) {}  // wait for high to low transition on clock
    while(digitalRead((CALIPER_CLOCK_PIN)==HIGH) ) {}  // wait for high to low transition on clock
  }
  if(mm)
  {
    temp_div = inches_caliper ? 100.0:78.74;
    result=(value*sign)/temp_div;
    //result=(value*sign)/(inches ? 100.0:78.74);
  }
  else
  {
  result=(value*sign)/(inches_caliper ? 2000.0:(100.0*25.4)); //We map the values for inches, define mm to false if you want inches values
  }

}





#endif // HAS_STATUS_MESSAGE
