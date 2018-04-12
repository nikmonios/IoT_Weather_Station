/***********************************************************************************
* Copyright (c) 2018 Nikos Monios -- moniosni@gmail.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
*OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
*OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************/

#include "TEWeatherShield.h"
#include <string> 

Serial pc(USBTX,USBRX);     // UART tx, rx (to be sent to PC for debug)
Serial Serial2(PA_2, PA_3); // Tx, Rx for esp8266 communication

static TEWeatherShield weatherShield; // load the shield

uint8_t minutes = 15; //sampling period, take 4 measurements per hour (4 * 15 = 60)

string Samples;

/**** functions prototypes ****/
void wait_some_time();
void construct_string_and_send( float htu21d_Temperature, float htu21d_Humidity, 
float ms5637_Temp, float ms5637_Press, float ms8607_temperature, float ms8607_pressure, 
float ms8607_humidity, float tsys01_temperature, float tsd305_temperature, 
float tsd305_object_temperature);


/********* MAIN **********/
int main(void)
{
    float htu21d_Temperature = 0;  // HTU21D temperature
    float htu21d_Humidity = 0;     // HTU21D humidity
    float ms5637_Temp = 0;         // MS5637 Temperature
    float ms5637_Press = 0;        // MS5637 Pressure
    float ms8607_temperature = 0;  // MS8607 temperature
    float ms8607_pressure = 0;     // MS8607 pressure
    float ms8607_humidity = 0;     // MS8607 humidity
    float tsys01_temperature = 0;  // TSYS01 temperature
    float tsd305_temperature = 0;  // TSD305 temperature
    float tsd305_object_temperature = 0; //TSD305 objective temperature
    
    pc.baud(9600); // set UART baud rate
    
    weatherShield.begin(); // begin the shield
    
    pc.printf("---- weather station is starting ----\r\n");
    wait(1);
    
    while(1)
    {
        /* read HTU21D */
        weatherShield.selectHTU21D(); //select the sensor
        pc.printf("============ HTU21D ============\r\n"); //for debug reasons
        weatherShield.HTU21D.set_i2c_master_mode(HTU21_i2c_no_hold); //set i2c master mode
        weatherShield.HTU21D.read_temperature_and_relative_humidity(&htu21d_Temperature, &htu21d_Humidity); //read the values
        pc.printf("temperatue from HTU21D is: %f \r\n", htu21d_Temperature); //print Temperature for debug
        pc.printf("humidity from HTU21D is : %f \r\n",  htu21d_Humidity); //print Humidity for debug
        wait(5);
        
        /* read MS5637 */
        weatherShield.selectMS5637(); //select the sensor
        pc.printf("============ MS5367 ============\r\n"); //for debug reasons
        weatherShield.MS5637.read_temperature_and_pressure(&ms5637_Temp, &ms5637_Press); //read the values
        pc.printf("temperatue from MS5637 is: %f \r\n", ms5637_Temp); //print Temperature for debug
        pc.printf("pressure from MS5637 is : %f \r\n",  ms5637_Press); //print Humidity  for debug
        wait(5);
        
        /* read MS8607 */
        weatherShield.selectMS8607();  //select the sensor
        pc.printf("============ MS8607 ============\r\n"); //for debug reasons
        weatherShield.MS8607.set_humidity_i2c_master_mode(ms8607_i2c_hold); //set i2c master mode
        weatherShield.MS8607.read_temperature_pressure_humidity( &ms8607_temperature, &ms8607_pressure, &ms8607_humidity); //read the values
        pc.printf("temperatue from MS8607 is: %f \r\n", ms8607_temperature); //print Temperature for debug
        pc.printf("pressure from MS8607 is : %f \r\n",  ms8607_pressure); //print Pressure for debug
        pc.printf("humidity from MS8607 is : %f \r\n",  ms8607_humidity); //print Humidity for debug
        wait(5);
        
        /* read TSYS01 */
        weatherShield.selectTSYS01(); //select the sensor
        pc.printf("============ TSY01 ============\r\n"); //for debug reasons
        weatherShield.TSYS01.read_temperature(&tsys01_temperature); //read the values
        pc.printf("temperatue from TSYS01 is: %f \r\n", tsys01_temperature); //print Temperature for debug
        wait(5);
        
        /* read TSD305 */
        weatherShield.selectTSD305(); //select the sensor
        pc.printf("============ TSD305 ============\r\n"); //for debug reasons
        weatherShield.TSD305.read_temperature_and_object_temperature(&tsd305_temperature, &tsd305_object_temperature); //read the values
        pc.printf("temperatue from TSD305 is: %f \r\n", tsd305_temperature); //print Temperature for debug
        pc.printf("Objective temperatue from TSD305 is: %f \r\n", tsd305_object_temperature); //print objective temperature for debug
        wait(5);
        
        
        /* construct the string that will send the info to ESP8266 */
        construct_string_and_send ( htu21d_Temperature, htu21d_Humidity, ms5637_Temp,
        ms5637_Press, ms8607_temperature, ms8607_pressure, ms8607_humidity, tsys01_temperature, 
        tsd305_temperature, tsd305_object_temperature );
        
        /* wait 15 minutes */
        wait_some_time();     
        
    } // end of while 
} // end of main

/********* FUNCTIONS **********/

void wait_some_time()
{
    int time = minutes * 60; // 15 * 60 = 900 seconds delay
    wait(time);
}

void construct_string_and_send( float htu21d_Temperature, float htu21d_Humidity, 
float ms5637_Temp, float ms5637_Press, float ms8607_temperature, float ms8607_pressure, 
float ms8607_humidity, float tsys01_temperature, float tsd305_temperature, 
float tsd305_object_temperature )
{
    /* variables to be converted to strings */
    char htu21d_Temp_str[16];
    char htu21d_Hum_str[16];
    char ms5637_Temp_str[16];
    char ms5637_Press_str[16];
    char ms8607_temp_str[16];
    char ms8607_pres_str[16];
    char ms8607_hum_str[16];
    char tsys01_temp_str[16];
    char tsd305_temp_str[16];
    char tsd305_obj_str[16];
    
    /* construct the string that will send the info to ESP8266 */
    sprintf(htu21d_Temp_str, "%1.4lf", htu21d_Temperature);
    sprintf(htu21d_Hum_str, "%1.4lf", htu21d_Humidity);
    sprintf(ms5637_Temp_str, "%1.4lf", ms5637_Temp);
    sprintf(ms5637_Press_str, "%1.4lf", ms5637_Press);
    sprintf(ms8607_temp_str, "%1.4lf", ms8607_temperature);
    sprintf(ms8607_pres_str, "%1.4lf", ms8607_pressure);
    sprintf(ms8607_hum_str, "%1.4lf", ms8607_humidity);
    sprintf(tsys01_temp_str, "%1.4lf", tsys01_temperature);
    sprintf(tsd305_temp_str, "%1.4lf", tsd305_temperature);
    sprintf(tsd305_obj_str, "%1.4lf", tsd305_object_temperature);
    
    Serial2.printf("HTU21D_TEMP:%s,HTU21D_HUM:%s,MS5637_TEMP:%s,MS5637_PRESS:%s,MS8607_TEMP:%s,MS8607_PRESS:%s,MS8607_HUM:%s,TSYS01_TEMP:%s,TSD305_TEMP:%s,TSD305_OBJ:%s\r\n"
    ,htu21d_Temp_str, htu21d_Hum_str, ms5637_Temp_str, ms5637_Press_str, ms8607_temp_str,
    ms8607_pres_str, ms8607_hum_str, tsys01_temp_str, tsd305_temp_str, tsd305_obj_str);
}