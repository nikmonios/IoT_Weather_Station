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
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "globalVariables.h"


WiFiClient client; //create a client object - raspberry pi


void connectWiFi()
{
  byte ledStatus = LOW;

  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  WiFi.begin(WiFiSSID, WiFiPSK);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;

    // Delays allow the ESP8266 to perform critical tasks
    // defined outside of the sketch. These tasks include
    // setting up, and maintaining, a WiFi connection.
    delay(100);
    // Potentially infinite loops are generally dangerous.
    // Add delays -- allowing the processor to perform other
    // tasks -- wherever possible.
  }
}

/////////////////////////////////////////////////////////////////
void initHardware()
{
  Serial.begin(9600);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}
/////////////////////////////////////////////////////////////////
void GetIP()
{
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) 
  {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }
}
/////////////////////////////////////////////////////////////////
void connectToServer()
{
  // attempt to connect to the server
  Serial.println("Attempting to connect to server");
  
  uint8_t tries = 0;
  
  while (client.connect(server, port) == false) 
  {
    Serial.print(".");
    if (tries++ > 100) 
    {
      Serial.println("\nThe server isn't responding");
      while(1);
    }
    delay(100);
  }

  //we've connected to the server by this point
  Serial.println("\nConnected to the server!");
}
/////////////////////////////////////////////////////////////////
void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print("Network Name: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
/////////////////////////////////////////////////////////////////
void process_data (const char * data)
{
  if (client.available()) 
  {
    client.println(data); //send the string to raspberry pi
  }
}
/////////////////////////////////////////////////////////////////
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
   
}
/////////////////////////////////////////////////////////////////
// here to process incoming serial data after a terminator received
void process_data (const char * data)
{
  float values[3] = {0, 0, 0};
  int i;

  Serial.println("after parsing...");
  char* command = strtok(data, ",");
  while (command != 0)
  {
      i = 0;
      // Split the command in two values
      char* separator = strchr(command, ':');
      if (separator != 0)
      {
        // Actually split the string in 2: replace ':' with 0
        *separator = 0;
        ++separator;
      }
      // Find the next command in input string
      command = strtok(0, ",");
      values[i] = atof(separator);
   }

   //we now have the data from the Nucleo Board.
   //Use MQTT to send them to Cayenne cloud
   data_to_cloud(values);
}  // end of process_data
/////////////////////////////////////////////////////////////////
void data_to_cloud(float *samples)
{

}
/////////////////////////////////////////////////////////////////
//EoF
