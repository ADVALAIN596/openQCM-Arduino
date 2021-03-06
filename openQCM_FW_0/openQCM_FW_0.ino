/* LICENSE
 * Copyright (C) 2014 openQCM
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
 * along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * INTRO
 * openQCM is the unique opensource quartz crystal microbalance http://openqcm.com/
 * openQCM Java software project is available on github repository 
 * https://github.com/marcomauro/openQCM
 * 
 * Measure QCM frequency using FreqCount library developed by Paul Stoffregen 
 * https://github.com/PaulStoffregen/FreqCount
 *
 * author     Marco Mauro 
 * version    0 (basic)
 * date       october 2014 
 *
 */
 
 #include <FreqCount.h>
 
 // fixed "gate interval" time for counting cycles 1000ms  
 #define GATE 1000
 
 // print data to serial port 
 void dataPrint(unsigned long Count, int Temperature){
  Serial.print("RAWMONITOR");
  Serial.print(Count);
  Serial.print("_");
  Serial.print(Temperature);
  Serial.write(255);
}

// map value to a double
double map_value 
(int x, int in_min, int in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// measure temperature
int getTemperature(void){
  // TODO calibration measure
  const int calib = - 32;
  const int R_ref = 100;
  double volt = map_value(analogRead(1), 0, 1023, 0, 5);
  double alpha = volt/5;
  double resistance = (alpha/(1 - alpha))*R_ref;
  // PT100 linear relation between resistance and temperature
  // t(°C) = 2,596*R(ohm) - 259,8
  // Resistance range (100, 140) OHM*/  
  int Temperature = int ( 2.596 * resistance - 259.8 );
  // TODO PT100 calibration measure 
  Temperature = Temperature + calib;
  return(Temperature);
}

// variable declaration
unsigned long frequency = 0;
int temperature = 0;

void setup(){
  Serial.begin(115200);
  FreqCount.begin(GATE);
}

void loop(){
  if (FreqCount.available()) 
  {
    frequency = FreqCount.read();       // measure frequency
    temperature = getTemperature();     // measure temperature 
    dataPrint(frequency, temperature);  // print data
  }
}
