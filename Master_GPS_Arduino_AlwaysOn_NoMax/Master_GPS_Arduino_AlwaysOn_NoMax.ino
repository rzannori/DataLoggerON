

/*PIN out
 * GPS : RX=D6, TX = D7 

 * SD : MISO = D9,MOSI=D10,SCK=D8, CS= D3

 */

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include<SD.h>

const float threshold = 0.7; // 70%
const int cs_sd=3; // pin di CS della SD
const int TState = 200;
static const int RXPin = 7, TXPin = 6; //comunicazione GPS
static const uint32_t GPSBaud = 9600;
int ledPin; //11 rosso , 12 blu,13 verde
int DLState;

int maxspeed, maxalt, maxsatellite;

TinyGPSPlus gps;// oggetto TinyGPSPlus 
SoftwareSerial ss(RXPin, TXPin);// connessione seriale al GPS

//------------------------------------------------------------------------------
void setup() {
 
 Serial.begin(9600);
 ss.begin(GPSBaud);
 while ( !SD.begin(cs_sd)){
       }
 pinMode(ledPin, OUTPUT);

 //ch.attach();
  }
//------------------------------------------------------------------------------
void loop() {
  Serial.begin(9600); 
int satellite = gps.satellites.value() ;

   if ( gps.speed.kmph() > maxspeed) {
    maxspeed = gps.speed.kmph();
    }
   if ( gps.altitude.meters() > maxalt) {
    maxalt=gps.altitude.meters();
   }
   if ( gps.satellites.value() > maxsatellite) {
     maxsatellite = gps.satellites.value();
    } 
if (satellite > 0){ 
   ledPin=13;
     
    String SpeedH=String(maxspeed);
    String AltH=String ( maxalt);
    String SatH=String (maxsatellite);
    String Time=String(gps.time.hour()+1)+(":")+(gps.time.minute())+(":")+(gps.time.second());
    String Date=String(gps.date.day())+("/")+(gps.date.month())+("/")+(gps.date.year());
      

              SD.remove ("DatiMax.txt");
              File DatiMax=SD.open("DatiMax.txt",FILE_WRITE);
              DatiMax.println(SpeedH + "," + AltH + "," + SatH);
              DatiMax.close();
              File data=SD.open("DatiVolo.txt",FILE_WRITE);
              data.println(Date + " , " + Time + " , " + String(gps.location.lng(), 6)+ " , " + String(gps.location.lat(), 6)+ " , " + String(gps.altitude.meters(),0)+ " , " + String(gps.speed.kmph(),0) + " , " + String(gps.satellites.value())); 
              data.close();
              digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
              delay(TState);                       // wait for a second
              digitalWrite(ledPin, HIGH);    // turn the LED off by making the voltage LOW
              delay(TState); 
          }
         
  else {
        ledPin=11;
        Serial.println ("looking");
        digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
        delay(TState); 
        digitalWrite(ledPin,HIGH );    // turn the LED off by making the voltage LOW
        delay(TState);
      }
 
  DelayGPS(500);
   
  }
 
 //==========================================sub routine=====================================
  static void DelayGPS(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
