#include "TEWeatherShield.h"

TEWeatherShield::TEWeatherShield(void) {}


void TEWeatherShield::begin(void) 
{
  selectSensor(Sensor_NONE);

  HTU21D.begin();
  MS5637.begin();
  MS8607.begin();
  TSYS01.begin();
  TSD305.begin();
}

void TEWeatherShield::selectSensor(enum TEWeatherShield_Sensor sensor) 
{
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);

  switch (sensor) 
  {
    case Sensor_HTU21D:
      digitalWrite(31, LOW);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      delay(5);
      break;
    case Sensor_MS5637:
      digitalWrite(31, LOW);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      delay(5);
      break;
    case Sensor_MS8607:
      digitalWrite(31, LOW);
      digitalWrite(32, LOW);
      digitalWrite(33, HIGH);
      delay(5);
      break;
    case Sensor_TSYS01:
      digitalWrite(31, LOW);
      digitalWrite(32, HIGH);
      digitalWrite(33, LOW);
      delay(5);
      break;
    case Sensor_TSD305:
      digitalWrite(31, LOW);
      digitalWrite(32, HIGH);
      digitalWrite(33, HIGH);
      delay(5);
      break;
    case Sensor_NONE:
      digitalWrite(31, HIGH);
      digitalWrite(32, HIGH);
      digitalWrite(33, HIGH);
      break;
  }
}

void TEWeatherShield::selectHTU21D() { selectSensor(Sensor_HTU21D); }

void TEWeatherShield::selectMS5637() { selectSensor(Sensor_MS5637); }

void TEWeatherShield::selectMS8607() { selectSensor(Sensor_MS8607); }

void TEWeatherShield::selectTSYS01() { selectSensor(Sensor_TSYS01); }

void TEWeatherShield::selectTSD305() { selectSensor(Sensor_TSD305); }
