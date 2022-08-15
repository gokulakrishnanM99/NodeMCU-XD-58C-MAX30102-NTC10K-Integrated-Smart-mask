# NodeMCU-XD-58C-MAX30102-NTC10K-Integrated-Smart-mask
Sensors that measure heart rate, respiration rate, blood pressure, and body temperature in real-time are integrated with ESP8266 inside a mask to monitor vital parameters.



# Materials required:

1) ESP8266 like NodeMCU.
2) XD-58C
3) MAX30100 integrated heart rate and pulse oximeter.
4) NTC 10K ohm
5) A system with Arduino software to upload the code
6) Connecting wires.
7) Power source & OTG cable 
8) Blynk Application

# Connections:

   NodeMCU --->>  XD-58C
  -----------------------
  
  GND        --->  Pin-1 (Black) - GND
  5V or 3.3V ---> Pin-2 (Red) - VCC
  O/p        ---> Pin-3 (Purple) - Signal
Note: Colour code might be change for sensors bought from a different company
  
  
   NodeMCU --->>  MAX30100
  -----------------------  
     3.3V/Vin --->   Vin 
     GND      --->   GND
     D1       --->   SCL
     D2       --->   SDA
   
     
   NodeMCU --->>  NTC 10K
  -----------------------  
     5V/Vin --->   Vcc
     GND    --->   GND
     A0     --->  Vcc / Analog O/p
     
  MULTIPLEXER - CD74HC4067 is used for selecting a sensor at a particular time. 
  Control pins: S0 to S3
  
**INTEGRATION OF MULTIPLE SENSORS WITH NODE MCU**
The Heart Rate sensor, Respiration Rate sensor and Pulse Oximeter are integrated with Node MCU by using a 16-channel M multiplexer sensor. 
C0  and C1, analogue input of 16 channel multiplexer is connected with the thermistor's positive pin and the pulse sensor's signal pin, respectively.
The signal pin SIG of the multiplexer is connected with the A0 Analog pin of Node MCU. 
The digital pins S0 and S1 of the multiplexer are connected with digital pins D0 and D4 of Node MCU, which act as a switch.
The SCL and SDA pins of the pulse oximeter are connected to digital pins D1 and D2 of Node MCU. 
The 3V power supply of the node MCU is connected at Vin of all the sensors. 


  **Software Interface**
  
Download the Blynk app on smartphone and create an account. 
Create a new project → Name the project → Copy the authentication token sent to the registered email id or copy from the app itself. 
In Arduino IDE, download the Blynk library from the Github website. (https://github.com/Blynkkk/Blynk-library) 
Arduino →Sketch→Include Library→Select Blynk library file.
File→Examples→Blynk→Boards Wifi→ESP8266 Standalone (to connect the Wifi to node MCU).
Paste the authentication token and enter the details (username and password) of Wifi connected to both phone and laptop (that houses Arduino IDE). 
Copy ESP8266 library address→File→Preferences→Paste the address→Ok.
Tools→Board→Board Manager→Select NodeMCU 0.9 (ESP 12 MODULE).
Select the port to which the USB from Node MCU is connected.
Run The Program by compiling and uploading the program to the board.
Open the serial monitor from the Tool option to see the displayed values.
Connect the sensors to get the values.
