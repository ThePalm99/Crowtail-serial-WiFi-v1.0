#include <SoftwareSerial.h>
 
#define DEBUG true
 
SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{
  Serial.begin(115200);
  esp8266.begin(115200); // esp start baud rate
  esp8266.print("AT+CIOBAUD=9600\r\n");
  delay(100);
  esp8266.begin(9600);
  delay(100);  
  sendData("AT+GMR\r\n",2000,DEBUG);// firmware info
  Serial.println("");
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  Serial.println("");
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  Serial.println("");
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  Serial.println("");
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80*/
}
 
void loop()
{
  delay(10000);
  if(esp8266.available()) // check if the esp is sending a message 
  {
       
    if(esp8266.find("+IPD,"))
    {
     delay(1000);
 
     int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
     
     String webpage = "<h1>Hello World! </h1>";
     
     String cipSend = "AT+CIPSEND=";
     
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
 
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=5; // append connection id
     closeCommand+="\r\n";    
     sendData(closeCommand,3000,DEBUG);
    }
  }
}
 
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    char ch;

    
    while( (time+timeout) > millis())
    {
      
      while(esp8266.available())
      {      
        // The esp has data so display its output to the serial window 
        ch = esp8266.read();
        //delay(1);// read the next character.
        response += ch;
      }  
    }

    if(debug)
    {
      Serial.print(response);
    }    
    return response;
}
 
