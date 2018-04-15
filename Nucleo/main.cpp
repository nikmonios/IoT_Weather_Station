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
Serial Serial2(D8, D2); // Tx, Rx for esp8266 communication

static TEWeatherShield weatherShield; // load the shield

uint8_t minutes = 15; //sampling period, take 4 measurements per hour (4 * 15 = 60)

string Samples;

/**** functions prototypes ****/
void wait_some_time();
void construct_string_and_send( float *samples_array );
void check_samples_state( float *samples_array );


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
    float Samples_Array[10];
    
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
        Samples_Array[0] = htu21d_Temperature;
        Samples_Array[1] = htu21d_Humidity;
        Samples_Array[2] = ms5637_Temp;
        Samples_Array[3] = ms5637_Press;
        Samples_Array[4] = ms8607_temperature;
        Samples_Array[5] = ms8607_pressure;
        Samples_Array[6] = ms8607_humidity;
        Samples_Array[7] = tsys01_temperature;
        Samples_Array[8] = tsd305_temperature;
        Samples_Array[9] = tsd305_object_temperature;
        
        /* chech the status of the sensors */
        check_samples_state( Samples_Array );
        
        /* if samples are OK, send the string to ESP8266 */
        construct_string_and_send ( Samples_Array );
        
        /* wait 15 minutes */
        wait_some_time();     
        
    } // end of while 
} // end of main

/***************** FUNCTIONS *****************/

/**
 * \brief stops system untill next sampling period
 *        
 *
 * \param[in] 
 *
 * \return 
 */
void wait_some_time()
{
    int time = minutes * 60; // 15 * 60 = 900 seconds delay
    wait(time);
}


/**
 * \brief fills string with samples and sends them to ESP8266
 *        
 *
 * \param[in] float : array that contains samples from sensors
 *
 * \return 
 */
void construct_string_and_send( float *samples_array )
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
    sprintf(htu21d_Temp_str, "%1.4lf", samples_array[0]);
    sprintf(htu21d_Hum_str, "%1.4lf", samples_array[1]);
    sprintf(ms5637_Temp_str, "%1.4lf", samples_array[2]);
    sprintf(ms5637_Press_str, "%1.4lf", samples_array[3]);
    sprintf(ms8607_temp_str, "%1.4lf", samples_array[4]);
    sprintf(ms8607_pres_str, "%1.4lf", samples_array[5]);
    sprintf(ms8607_hum_str, "%1.4lf", samples_array[6]);
    sprintf(tsys01_temp_str, "%1.4lf", samples_array[7]);
    sprintf(tsd305_temp_str, "%1.4lf", samples_array[8]);
    sprintf(tsd305_obj_str, "%1.4lf", samples_array[9]);
    
    Serial2.printf("HTU21D_TEMP:%s,HTU21D_HUM:%s,MS5637_TEMP:%s,MS5637_PRESS:%s,MS8607_TEMP:%s,MS8607_PRESS:%s,MS8607_HUM:%s,TSYS01_TEMP:%s,TSD305_TEMP:%s,TSD305_OBJ:%s\r\n"
    ,htu21d_Temp_str, htu21d_Hum_str, ms5637_Temp_str, ms5637_Press_str, ms8607_temp_str,
    ms8607_pres_str, ms8607_hum_str, tsys01_temp_str, tsd305_temp_str, tsd305_obj_str);
}


/**
 * \brief resets the board if a sensor is malfunctioning
 *        
 *
 * \param[in] float : array that contains samples from sensors
 *
 * \return 
 */
void check_samples_state( float *samples_array )
{
    uint8_t flag = 0; // flags a sensor malfunction
    
    if(samples_array[0] == 0 && samples_array[1] == 0) // is 1st sensor ok?
    {
        flag = 1; // if not, raise flag
    }
    if(samples_array[2] == 0 && samples_array[3] == 0) // is 2nd sensor ok?
    {
        flag = 1; // if not, raise flag
    }
    if(samples_array[4] == 0 && samples_array[5] == 0 && samples_array[6] == 0) // is 3rd sensor ok?
    {
        flag = 1; // if not, raise flag
    }
    if(samples_array[7] == 0) // is 4th sensor ok?
    {
        flag = 1; // if not, raise flag
    }
    if(samples_array[8] == 0 && samples_array[9] == 0) // is 5th sensor ok?
    {
        flag = 1; // if not, raise flag
    }
    
    //now check the status of the flag
    if(flag == 0) // if flag is down
    {
        return; // return to program execution
    }
    else // if we have a malfunction
    {
        pc.printf("WARNING: A sensor is malfunctioning\r\n"); //for debug reasons
        pc.printf("***** Board is restarting *****\r\n"); //for debug reasons
        pc.printf("\r\n"); //for debug reasons
        NVIC_SystemReset(); //reset the board
    }
        
}