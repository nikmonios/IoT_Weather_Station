/**********************
* TO DO:
* add last sensor
* 
***********************/

#include "TEWeatherShield.h"

Serial pc(USBTX,USBRX);     // UART tx, rx (to be sent to PC for logging, and later to esp8266

static TEWeatherShield weatherShield; // load the shield

void HTU21D(void);
void MS5367(void);
void MS8607(void);
void TSY01(void);


int main(void)
{
    pc.baud(9600); // set UART baud rate
    
    weatherShield.begin(); // begin the shield
    
    pc.printf("---- weather station is starting ----\r\n");
    wait(1);
    
    while(1)
    {
        HTU21D(); //read htu21d and print them
        wait(5);
        
        MS5367(); //read ms5637 and print them    
        wait(5);
        
        MS8607(); //read ms8607 and print them     
        wait(5);
        
        TSY01(); //read tsys01 and print them
        wait(5);
    } // end while 
}

/****************  FUNCTIONS ****************/
void HTU21D(void)
{
    float htu21d_Temperature = 0;              // variable for HTU21D temperature in degrees C
    float htu21d_Humidity = 0;                 // variable for HTU21D humidity
    
    weatherShield.selectHTU21D();
    pc.printf("============ HTU21D ============\r\n");
    weatherShield.HTU21D.set_i2c_master_mode(htu21_i2c_no_hold);       
    weatherShield.HTU21D.read_temperature_and_relative_humidity(&htu21d_Temperature, &htu21d_Humidity); 
    pc.printf("temperatue from HTU21D is: %f \r\n", htu21d_Temperature); //print Temperature
    pc.printf("humidity from HTU21D is : %f \r\n",  htu21d_Humidity); //prin Humidity
}


void MS5367(void)
{
    // MS5367 temperature and pressure variables
    float ms5637_Temp = 0;
    float ms5637_Press = 0;
    
    weatherShield.selectMS5637();
    pc.printf("============ MS5367 ============\r\n");       
    weatherShield.MS5637.read_temperature_and_pressure(&ms5637_Temp, &ms5637_Press);       
    pc.printf("temperatue from MS5637 is: %f \r\n", ms5637_Temp); //print Temperature
    pc.printf("pressure from MS5637 is : %f \r\n",  ms5637_Press); //prin Humidity
}


void MS8607(void)
{
    float ms8607_temperature = 0;              // MS8607 temperature variable in degress C
    float ms8607_pressure = 0;                 // MS8607 pressure variable
    float ms8607_humidity = 0;                 // MS8607 humidity variable
    
    weatherShield.selectMS8607(); 
    pc.printf("============ MS8607 ============\r\n");      
    weatherShield.MS8607.set_humidity_i2c_master_mode(ms8607_i2c_hold); 
    weatherShield.MS8607.read_temperature_pressure_humidity( &ms8607_temperature, &ms8607_pressure, &ms8607_humidity);     
    pc.printf("temperatue from MS8607 is: %f \r\n", ms8607_temperature); //print Temperature
    pc.printf("pressure from MS8607 is : %f \r\n",  ms8607_pressure); //prin Pressure
    pc.printf("humidity from MS8607 is : %f \r\n",  ms8607_humidity); //prin Humidity
}


void TSY01(void)
{
    float tsys01_temperature = 0;              // tsys01 temperature variable
    
    weatherShield.selectTSYS01();
    pc.printf("============ TSY01 ============\r\n");
    weatherShield.TSYS01.read_temperature(&tsys01_temperature);
    pc.printf("temperatue from TSYS01 is: %f \r\n", tsys01_temperature); //print Temperature
}