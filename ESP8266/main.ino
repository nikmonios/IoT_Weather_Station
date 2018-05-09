/*************************************************************************
    Copyright (c) 2017 Nikos Monios
    
    This file is part of Smart-Garden.

    Smart-Garden is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Smart-Garden is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Smart-Garden.  If not, see <http://www.gnu.org/licenses/>.

    Athens, Greece
 **************************************************************************/
#include "functions.h"

const unsigned int MAX_INPUT = 128;

void setup() 
{
  initHardware(); //init pins, serial etc
  
  connectWiFi(); //init wi-fi connection. blinking led indicates that the connection is being established

  GetIP(); //get an IP for this device. Maybe it is not needed after all - will check again in the future
  Serial.println("\nIP Address obtained");

  printWifiStatus(); //print our connection status info
  
  connectToServer(); //connect to raspberry
}

void loop()
{
  while (Serial.available () > 0) //if TivaC has sent us something
    processIncomingByte (Serial.read ()); //then read it and send it over Wi-Fi to raspberry pi
}
/************************************************************************/

void processIncomingByte (const byte inByte)
{
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
    {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte
      
      // terminator reached! process input_line here ...
      process_data (input_line);
      
      // reset buffer for next time
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

    }  // end of switch
   
} // end of processIncomingByte  
//EoF

