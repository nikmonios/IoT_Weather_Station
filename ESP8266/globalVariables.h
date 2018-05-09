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
 
// WiFi Definitions
const char WiFiSSID[] = "USE_CASE_SSD";
const char WiFiPSK[] = "passw0rd!";

// Pin Definitions
const int LED_PIN = 5; // Thing's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the Thing
const int DIGITAL_PIN = 12; // Digital pin to be read

// client info
uint16_t port = 6880;     // port number of the raspberry
IPAddress server(192, 168, 0, 102);   // IP Address of the raspberry

// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 128;

//EoF
