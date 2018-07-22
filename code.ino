#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial esp8266(9,10); // This makes pin 9 of Arduino as RX pin and pin 10 //of Arduino as the TX pin
    • include the library code:

#include <LiquidCrystal.h>

float conduct; // initialize the library by associating any needed LCD interface pin

// with the arduino pin number it is connected to

const int sensorPin= 0;

int water_quality;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {

pinMode(8, OUTPUT);

lcd.begin(16, 2);	// set up the LCD's number of columns and rows

lcd.setCursor (0,0);	//Print a message to the LCD.

lcd.print ("Water Pollution");

lcd.setCursor (0,1);

lcd.print ("Monitoring System");

delay(1000);

Serial.begin(115200);

esp8266.begin(115200);

sendData("AT+RST\r\n",2000,DEBUG); // reset module

sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point

sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address

sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections

//sendData("AT+CIPMUwater_quality=1\r\n",1000,DEBUG);

connections

sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80

pinMode(sensorPin, INPUT);

lcd.clear();

}

void loop() {

conduct = analogRead(A2);

float water_quality = conduct*(5.0/1023.0);

//Serial.println(water_quality);

delay(1000); if(esp8266.available())


// check if the esp is sending a message


if(esp8266.find("+IPD,"))

{

delay(1000);

int connectionId = esp8266.read()-48; // We are subtracting 48 from the output because the read() function returns the ASCII decimal value and the first decimal number which is 0 starts at 48

String webpage = "<script> console.log('herer'); </script><h1>IOT based Water Pollution Monitoring System</h1>";

webpage += "<p><h2>";

webpage+= " Water Quality is ";

webpage+= water_quality;

webpage+=" micro-Siemens per cm or uS/cm";

webpage += "<p>";

if (water_quality<=4.8)

{

webpage+= "Good water";

}

else if(water_quality<=4.9 && water_quality>=4.8)

{

webpage+= "Bad Water";
}

else if (water_quality>=4.91 )

{

webpage+= "Danger! Dont Drink this Water";

}


webpage += "</h2></p></body>";

String cipSend = "AT+CIPSEND=";

cipSend += connectionId;

cipSend += ",";

cipSend +=webpage.length();

cipSend +="\r\n";


sendData(cipSend,1000,DEBUG);

sendData(webpage,1000,DEBUG);


cipSend = "AT+CIPSEND=";

cipSend += connectionId;

cipSend += ",";

cipSend +=webpage.length();

cipSend +="\r\n";


String closeCommand = "AT+CIPCLOSE=";

closeCommand+=connectionId; // append connection id closeCommand+="\r\n";


sendData(closeCommand,3000,DEBUG);

}

}

lcd.setCursor (0, 0);

lcd.print (" Water Q=");
lcd.print (water_quality);

lcd.print (" S/cm");

lcd.setCursor (0,1);

if (water_quality<=4.8)

{

lcd.print(" Distilled water");

digitalWrite(8, LOW);

}

else if( water_quality>=4.8 && water_quality<=4.9 )

{

lcd.print(" Drinking H2O");

digitalWrite(8, LOW );

}

else if (water_quality>=4.91 )

{

lcd.print(" Dirty H2O");

digitalWrite(8, HIGH);	// turn the LED on

}

lcd.scrollDisplayLeft();

delay(2000);

lcd.clear();

}

String sendData(String command, const int timeout, boolean debug)

{

String response = "";

esp8266.print(command); // send the read character to the esp8266 long int time = millis();

while( (time+timeout) > millis())

{

while(esp8266.available())

{// The esp has data so display its output to the serial window
char c = esp8266.read();	// read the next character.

response+=c;

}

}

if(debug)

{

Serial.print(response);

}

return response;

}
