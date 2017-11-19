#include "TEWeatherShield.h"

TEWeatherShield::TEWeatherShield(void): HTU21D(D14, D15), MS5637(D14, D15){} //constructor for sensors, pins are sda and scl


void TEWeatherShield::begin(void) 
{
  selectSensor(Sensor_NONE);

  //HTU21D.begin();
  //MS5637.begin();
  //MS8607.begin();
  //TSYS01.begin();
  //TSD305.begin();
}

void TEWeatherShield::selectSensor(enum TEWeatherShield_Sensor sensor) 
{
  DigitalOut myled0(D9);
  DigitalOut myled1(D10);
  DigitalOut myled2(D11);

  switch (sensor) 
  {
    case Sensor_HTU21D:
      myled0 = 0;
      myled1 = 0;
      myled2 = 0;
      wait(0.005);
      break;
    case Sensor_MS5637:
      myled0 = 0;
      myled1 = 0;
      myled2 = 0;
      wait(0.005);
      //delay(5);
      break;
    /*case Sensor_MS8607:
      //digitalWrite(31, LOW);
      //digitalWrite(32, LOW);
      //digitalWrite(33, HIGH);
      delay(5);
      break;
    case Sensor_TSYS01:
      ///digitalWrite(31, LOW);
      //digitalWrite(32, HIGH);
      //digitalWrite(33, LOW);
      delay(5);
      break;
    case Sensor_TSD305:
      //digitalWrite(31, LOW);
      //digitalWrite(32, HIGH);
      //digitalWrite(33, HIGH);
      delay(5);
      break;*/
    case Sensor_NONE:
      //digitalWrite(31, HIGH);
      //digitalWrite(32, HIGH);
      //digitalWrite(33, HIGH);
      break;
  }
}

void TEWeatherShield::selectHTU21D() { selectSensor(Sensor_HTU21D); }

void TEWeatherShield::selectMS5637() { selectSensor(Sensor_MS5637); }

//void TEWeatherShield::selectMS8607() { selectSensor(Sensor_MS8607); }

//void TEWeatherShield::selectTSYS01() { selectSensor(Sensor_TSYS01); }

//void TEWeatherShield::selectTSD305() { selectSensor(Sensor_TSD305); }