# IoT_Weather_Station
<b><i>Create an IoT application using a F103RB Nucleo ARM board, an Arduino wather shield from TE Connectivity, an ESP8266 thing and a Raspberry PI</i></b>

<br>
<b>WORK IN PROGRESS!!!</b>
<br>

<br><strong>Development boards used: </strong>
1) STM32 F103RB Nucleo board - http://www.st.com/en/evaluation-tools/nucleo-f103rb.html
2) Raspberry PI 3 - Model B  - https://www.adafruit.com/products/3055
3) Sparkfun ESP8266 Thing - https://www.sparkfun.com/products/13231
4) Arduino Weather Station - http://www2.mouser.com/ProductDetail/TE-Connectivity/DPP902S000/?qs=1fQ7IOp4WrheKF877Di7Eg%3d%3d


<b>What does it do?</b>
<br>
This IoT application uses a weather station from TE Connectiviy to measure physical parameters (Temperature, Pressure and Humidity) using various sensors. Originally, TE Connectivity has released the source code of the shield's library for Arduino, but I decided to re-write it for ARM-based Nucleo boards, using the mbed online compiler. Please note, that you can use the Arduino IDE to download the stm32duino and program ARM-based boards with the Arduino C++, but I wanted a challenge, and I used mbed c++.
<br>
<br>
So, the Nucleo F103RB is the board that will host the weather shield. It responsible for deciding when to sleep, when to take samples and when to transmit samples. In order for the application to communicate with the internet, an ESP8266 thing was used, which communicates serially with the Nucleo board. The Nucleo F103RB sends samples 4 times per hour (every 15 minutes) to the ESP8266, and the ESP8266 sends them to the Cayenne Cloud.
<br>
<br>
The Raspberry Pi will be used to download the historical data from the Cayenne Cloud and make a forecast for the weather of the vicinity in the near future. The predictions will be re-uploaded to the cloud so the user can see them from any place in the planet - as long as there is an internet connection!
<br>
This picture shows the topology of the application:
<br>
![alt text][logo]
[logo]: https://www.dropbox.com/s/mzihgbuv0aezqul/topology.png?dl=0 "topology of application"

<br>
<strong>Flashing the boards: </strong>
<br>To flash the F103RB board, I used the cloud-based mbded IDE compiler. You can create a free account on https://os.mbed.com/ , and then go to the compiler page https://os.mbed.com/compiler, where all your projects are saved. When you compile your project, it is automatically downloaded to your system, and you can drag and drop it to the Nucleo board, in order to start the flashing.


<br>To flash the ESP8266, I used the Arduino IDE. Instructions on how to setup the Arduino IDE to support the ESP8266 can be found on https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide
