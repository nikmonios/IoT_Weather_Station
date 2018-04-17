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
#include "parameters_struct.h"
#include <string> 


Serial pc(USBTX, USBRX);     // UART tx, rx (to be sent to PC for debug)
Serial Serial_ESP(D8, D2);

static TEWeatherShield weatherShield; // load the shield

uint8_t minutes = 15; //sampling period in minutes, take 4 measurements per hour (4 * 15 = 60)

string Samples;

/**** functions prototypes ****/
void wait_some_time();
void construct_string_and_send( parameters_struct &samples );
void check_samples_state( float *samples_array );
void mean_of_samples( float *samples_array, parameters_struct &samples);


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
    float Samples_Array[10];       // store samples of each sensor here (temp & hum from 1st, temp & press from 2nd etc)
    parameters_struct parameters;  // struct that stores the values of Temp, Press and Hum that will be transmitted to ESP8266
    
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
        
        /* find average from each sensor */
        mean_of_samples( Samples_Array, parameters);
        
        /* if samples are OK, send the string to ESP8266 */
        construct_string_and_send ( parameters );
        
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
void construct_string_and_send( parameters_struct &samples )
{
    /* variables to be converted to strings */
    char Temperature[16];
    char Humidity[16];
    char Pressure[16];

    /* convert samples to strings */
    sprintf(Temperature, "%1.4lf", samples.temperature);
    sprintf(Humidity, "%1.4lf", samples.humidity);
    sprintf(Pressure, "%1.4lf", samples.pressure);
    
    /* send them to ESP using serial com */
    Serial_ESP.printf("TEMPERATURE:%s,HUMIDITY:%s,PRESSURE:%s\r\n",Temperature, Humidity, Pressure);
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
    
    if(samples_array[0] == 0 && samples_array[2] == 0 && samples_array[4] == 0 && samples_array[7] == 0 && samples_array[8] == 0) // have we got at least one temperature sample?
    {
        flag = 1; // if not, raise flag
    }
    if(samples_array[1] == 0 && samples_array[6] == 0) // have we got at least one humidity sample?
    {
        flag = 1; // if not, raise flag
    }
    if(samples_array[3] == 0 && samples_array[5] == 0) // have we got at least one pressure sample?
    {
        flag = 1; // if not, raise flag
    }

    
    //now check the status of the flag
    if(flag == 0) // if flag is down
    {
        return; // return to program execution
    }
    else // if we have at least one malfunction
    {
        pc.printf("WARNING: A sensor is malfunctioning\r\n"); //for debug reasons
        pc.printf("***** Board is restarting *****\r\n"); //for debug reasons
        pc.printf("\r\n"); //for debug reasons
        NVIC_SystemReset(); //reset the board
    }
        
}


/**
 * \brief finds average of each parameter from every sensor
 *        
 *
 * \param[in] float : array that contains samples from sensors
 *
 * \return    structure that contains average of every physical parameter
 */
void mean_of_samples( float *samples_array, parameters_struct &samples)
{
    float avg_temperature = 0; /* average temperature that will be stored in struct */
    float avg_humidity = 0; /* average humidity that will be stored in struct */
    float avg_pressure = 0; /* average pressure  that will be stored in struct */
    
    /* arrays that are filled with all the samples from each physical parameter */
    float temp[5] = {samples_array[0], samples_array[2], samples_array[4], samples_array[7], samples_array[8]};
    float hum[2] = {samples_array[1], samples_array[6]};
    float pres[2] = {samples_array[3], samples_array[5]};
    
    uint8_t i = 0; /* counter */
    
    uint8_t div = 0; /* divisor */
    float sum = 0; /* temporary sum */
    
    /* find the average temperature from all working sensors */
    for(i = 0; i < 5; i++)
    {
        if(temp[i] != 0) /* if a sensor is not working, don't add it*/
        {
            sum += temp[i];
            div++; /* update divisor */
        }
    }
    avg_temperature = sum / div; /* calculate average temperature */
    
    /* reset sum and div */
    div = 0;
    sum = 0;
    
    /* find the average humidity from all working sensors */
    for(i = 0; i < 2; i++)
    {
        if(hum[i] != 0) /* if a sensor is not working, don't add it*/
        {
            sum += hum[i];
            div++; /* update divisor */
        }
    }
    avg_humidity = sum / div; /* calculate average humidity */
    
    /* reset sum and div */
    div = 0;
    sum = 0;
    
    /* find the average pressure from all working sensors */
    for(i = 0; i < 2; i++)
    {
        if(pres[i] != 0) /* if a sensor is not working, don't add it*/
        {
            sum += pres[i];
            div++; /* update divisor */
        }
    }
    avg_pressure = sum / div; /* calculate average pressure */
    
    /* update the struct with the average values */
    samples.temperature = avg_temperature;
    samples.humidity = avg_humidity;
    samples.pressure = avg_pressure;
}