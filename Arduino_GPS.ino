#include <SoftwareSerial.h>
SoftwareSerial sim808(10,11);

char phone_no[] = "09173174911"; 
String data[5];
#define DEBUG true
String state,timegps,latitude,longitude,year,month,day,hour,minute;

void setup() {
sim808.begin(9600);
Serial.begin(9600);
delay(50);

sim808.print("AT+CSMP=17,167,0,0");  
delay(100);
sim808.print("AT+CMGF=1\r");
delay(400);

sendData("AT+CGNSPWR=1",1000,DEBUG);
delay(50);
sendData("AT+CGNSSEQ=RMC",1000,DEBUG);
delay(150);

}

void loop() {
  sendTabData("AT+CGNSINF",1000,DEBUG);
  if (state !=0) {  
//  Serial.println("State  :"+state);
//  Serial.println("Time  :"+timegps);
  Serial.println("Year  :"+year);
  Serial.println("Month :"+month);
  Serial.println("Day :"+day);
  Serial.println("Hour  :"+hour);
  Serial.println("Minute  :"+minute);
  Serial.println("Latitude  :"+latitude);
  Serial.println("Longitude  :"+longitude);
  
//  sim808.print("AT+CMGS=\"");
//  sim808.print(phone_no);
//  sim808.println("\"");

  sim808.println();

  delay(300);

  sim808.print("http://maps.google.com/maps?q=loc:");
  sim808.print(latitude);
  sim808.print(",");
  sim808.print (longitude);
  delay(200);
//  sim808.println((char)26); 
  delay(200);
  sim808.println();
  sim808.println();
  sim808.println();
  sim808.println();
  delay(10000);
  sim808.flush();


} else {
  Serial.println("GPS Initializing…");
  }
}

void sendTabData(String command , const int timeout , boolean debug){
  sim808.println(command);
  long int time = millis();
  int i = 0;
  while((time+timeout) > millis()){
    while(sim808.available()){
      char c = sim808.read();
      if (c != ',') {
        data[i] +=c;
        delay(100);
      } 
      else {
        i++;
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
  }exitL:
  if (debug) {
    state = data[1];
    timegps = data[2];
    latitude = data[3];
    longitude =data[4];
    year = timegps.substring(0,4);
    month = timegps.substring(4,6);
    day = timegps.substring(6,8);
    int timezone = (timegps.substring(8,10)).toInt() + 8;
    hour = String(timezone);
    minute = timegps.substring(10,12);
  }
}
String sendData (String command , const int timeout ,boolean debug)
{
  String response = "";
  sim808.println(command);
  long int time = millis();
  int i = 0;
  
  while ( (time+timeout ) > millis()){
    while (sim808.available()){
    char c = sim808.read();
    response +=c;
    }
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}
