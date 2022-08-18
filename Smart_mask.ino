

// Extend Analog Inputs for NodeMCU board using 16 channel Analog Multiplexer

// Note Summary
// Note : Safety is very important when dealing with electricity. We take no responsibilities while you do it at your own risk.
// Note : NodeMCU is a very famous micro-controller with Wifi function which is commonly used for Internet Of Thing (IOT) application. 
// Note : However,NodeMCU only has one analog input which only allow to read one analog sensor.  
// Note : This code is made practical to convert 1 Analog Input to up to 16 Analog Inputs using the 16-channel Analog Multiplexer Module.
// Note : The main code for 16 channel Analog Multiplexer was Written by Sachin Soni for techiesms YouTube channel & community (https://www.youtube.com/watch?v=02spXPoa_Y8)
// Note : Solarduino is only to modify the code to suit our own undertanding and application.
 
/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/////////////*/


    /* 0- General */
#include <CD74HC4067.h>


#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>

#include <Wire.h> 
#include "MAX30105.h"
#include "heartRate.h"
#include <ESP8266WiFi.h>

MAX30105 particleSensor;

#include <Blynk.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

char auth[] = ""; /*Authentication*/
char ssid[] = ""; /*WiFi id*/
char pass[] = ""; /*WiFi Password*/

int bpm;
const int PulseWire = 0;      
int Threshold = 550;        
                               
PulseSensorPlayground pulseSensor;  

enum respState {
    Inhaling,
    Exhaling
};
unsigned long prevRespTime = 0;
unsigned long currRespTime = 0;
respState currentRespState;
int movingAvgRespRate = 0;
float avgbr = 0;
int count = 0;

long samplesTaken = 0; //Counter for calculating the Hz or read rate
long unblockedValue; //Average IR at power up
long startTime; //Used to calculate measurement rate

int perCent; 
int degOffset = 0.5; //calibrated Farenheit degrees
int irOffset = 1800;



    #define S0 D0                             /* Assign Multiplexer pin S0 connect to pin D0 of NodeMCU */
    #define S1 D4
    #define SIG A0                            /* Assign SIG pin as Analog output for all 16 channels of Multiplexer to pin A0 of NodeMCU */

    int decimal = 2;                          // Decimal places of the sensor value outputs 
                             /* Assign the name "sensor0" as analog output value from Channel C0 */
    int sensor1;                            /* Assign the name "sensor1" as analog output value from Channel C1 */
    int sensor2;                            /* Assign the name "sensor2" as analog output value from Channel C2 */
                               /* Assign the name "sensor3" as analog output value from Channel C3 */
 
void setup() {                                /* Put your codes here to run only once during micro controller startup */

    /* 0- General */
    
    pinMode(S0,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin SO */        
    pinMode(S1,OUTPUT);
    pinMode(SIG, INPUT);                      /* Define analog signal pin as input or receiver from the Multiplexer pin SIG */  
    
    Serial.begin(115200);                     /* to display readings in Serial Monitor at 9600 baud rates */
    Blynk.begin(auth, ssid, pass);

    pulseSensor.analogInput(PulseWire);  
    pulseSensor.setThreshold(Threshold); 
      
      Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  //The LEDs are very low power and won't affect the temp reading much but
  //you may want to turn off the LEDs to avoid any local heating
  particleSensor.setup(0); //Configure sensor. Turn off LEDs
  particleSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required.
  
  //Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 25; //Options: 0=Off to 255=50mA=0xFF hexadecimal. 100=0x64; 50=0x32 25=0x19
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 2048; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  particleSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required. 
 
}

void loop() { /* Put your codes here to run over and over again endlessly */
  
 
 
    /* 0- General */

   // Channel 0 (C0 pin - binary output 0,)
    digitalWrite(S0,LOW); 
    digitalWrite(S1,LOW); 
      delay(4000);
      sensor1 = analogRead(SIG);   
    
    if (sensor1 > 305)
    {
         
        currentRespState = Inhaling;
        prevRespTime = currRespTime;  // save time from last cycle
        currRespTime = millis();
       
        float period_respiration = currRespTime - prevRespTime;
        float breathrate = 60000 / period_respiration;

        count = count + 1;
        avgbr = breathrate + avgbr;
        float avg = avgbr / count;
        //Serial.println(count);
          Serial.print("Respirations per minute: ");
          Serial.println(avg);
          if(avg>25)
  { 
    Blynk.email("email_id","Respiration Rate ", "alret | Respiration Rate is HIGH");
  }
          Blynk.virtualWrite(V1,avg);
        
 
    }
    else if (sensor1 < 305)
    {
       
        currentRespState = Exhaling;
    }
    
  
    // Channel 1 (C1 pin - binary output 1)
    digitalWrite(S0,HIGH);
    digitalWrite(S1,LOW); 
  
    int myBPM = pulseSensor.getBeatsPerMinute(); 
    sensor2=analogRead(SIG);
    bpm=sensor2/8;
   Serial.print("Heart Rate : ");
   Serial.println(bpm);
   if(bpm<60 || bpm>100)
  { 
    Blynk.email("email_id"," Heartrate", "alret | Abnormal Heartrate");
  }

  long irValue = particleSensor.getIR();
  perCent = irValue / irOffset;
  Serial.print("SPO2=");
  Serial.print(perCent);
  Serial.print("%");
  if(perCent<   75)
  { 
    Blynk.email("email_id","Blood Oxygen Saturation", "alret | Blood Oxygen Saturation is LOW");
  }
  //Serial.print((float)samplesTaken / ((millis() - startTime) / 1000.0), 2);

  float temperatureF = particleSensor.readTemperatureF(); //Because I am a bad global citizen
  temperatureF = temperatureF + degOffset;
  
  Serial.print(" Temp(F)=");
  Serial.print(temperatureF, 2);
  Serial.print("F");
  if(temperatureF>99)
  { 
    Blynk.email("email_id","Temperature", "alret | Temperature is HIGH  ");
  }
  

  if (irValue < 50000) {
    Serial.print(" No finger?");
 } 
 Serial.println();
  Blynk.virtualWrite(V0,bpm);
  Blynk.virtualWrite(V2,perCent);
  Blynk.virtualWrite(V3,temperatureF);
  Blynk.run();
  }
