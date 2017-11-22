/**********************
* TO DO:
* add more sensors
* 
***********************/

#include "TEWeatherShield.h"

Serial pc(USBTX,USBRX);//UART tx, rx (to be sent to PC for logging, and later to esp8266
static TEWeatherShield weatherShield; //load the shield


int main(void)
{
    
    float htu21d_Temperature = 0; //variable for HTU21D temperature in C
    int htu21d_Humidity = 0; //variable for HTU21D humidity
    double ms5637_Temp = 0, ms5637_Press = 0; // MS5367 temperature and pressure variables
    float ms8607_temperature = 0;
    float ms8607_pressure = 0;
    float ms8607_humidity = 0;
    
    pc.baud(9600); //set UART baud rate
    weatherShield.begin(); //begin the shield
    
    pc.printf("---- weather station is starting ----\n");
    wait(1);
    
    while(1)
    {
        if (weatherShield.HTU21D.is_connected()) //if we have connected to the sensor
        {
                htu21d_Temperature = weatherShield.HTU21D.sample_ctemp(); //read the temperature value
                htu21d_Humidity = weatherShield.HTU21D.sample_humid(); //read the humidity value
                
                pc.printf("temperatue from HTU21D is: %f \r\n",htu21d_Temperature); //print Temperature
                pc.printf("humidity from HTU21D is : %d \r\n",htu21d_Humidity); //prin Humidity
                
                wait(5); //wait 5 seconds
                
                ms5637_Temp = weatherShield.MS5637.calcTemp(); //read ms5637 temperature
                ms5637_Press = weatherShield.MS5637.calcPressure(); //read ms5637 pressure
                
                pc.printf("temperatue from MS5637 is: %f \r\n",ms5637_Temp); //print Temperature
                pc.printf("pressure from MS5637 is : %f \r\n",ms5637_Press); //prin Humidity
                
                
                wait(5); //wait 5 seconds
                
                weatherShield.MS8607.set_humidity_i2c_master_mode(ms8607_i2c_hold); 
                weatherShield.MS8607.read_temperature_pressure_humidity( &ms8607_temperature, &ms8607_pressure, &ms8607_humidity);
                
                pc.printf("temperatue from MS8607 is: %f \r\n",ms8607_temperature); //print Temperature
                pc.printf("pressure from MS8607 is : %f \r\n",ms8607_pressure); //prin Pressure
                pc.printf("humidity from MS8607 is : %f \r\n",ms8607_humidity); //prin Humidity
                
                wait(10); //wait 10 seconds
        }
    }
}