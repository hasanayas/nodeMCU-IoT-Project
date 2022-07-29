#include <ESP8266WiFi.h>

//CONFİG FILE **************************************


#define IO_USERNAME  "your name"
#define IO_KEY       "your key"

#define WIFI_SSID "your ssid"
#define WIFI_PASS "your password"

#include "AdafruitIO_WiFi.h"

#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) ||         \
    defined(ADAFRUIT_PYPORTAL)

#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant

#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS,
                   NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
#endif

//*****************************************************


// Enter your wifi network name and Wifi Password
const char* ssid = "your ssid";
const char* password = "your pass";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Defining LED states and assigning initial values

String outputRedState = "off";
String outputGreenState = "off";
String outputYellowState = "off";
String outputBlueState = "off";

//  Tanımlamalar yapıldı
int count = 0;
int count2 = 0; 
int count3 = 0; 
int count4 = 0;

AdafruitIO_Feed *counter = io.feed("counter");
AdafruitIO_Feed *counter2 = io.feed("counter2");
AdafruitIO_Feed *counter3 = io.feed("counter3");
AdafruitIO_Feed *counter4 = io.feed("counter4");

int buzzer =D1;
const int redLED = D7;
const int greenLED = D5;
const int yellowLED = D6;
const int blueLED = D2;
int hareketSensor = D3; //pır sensor pinout

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000 

;

void setup() {

Serial.begin(115200);
  Serial.print("Adafruit'e baglaniyor");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Connection complete
  Serial.println();
  Serial.println(io.statusText());
  
Serial.begin(115200);
// Çıkışlar belirtildi
pinMode(redLED, OUTPUT);
pinMode(greenLED, OUTPUT);
pinMode(yellowLED,OUTPUT);
pinMode(blueLED, OUTPUT);
pinMode(hareketSensor, INPUT);
pinMode(buzzer,OUTPUT);
  
// LEDs go out at first
digitalWrite(redLED, LOW);
digitalWrite(greenLED, LOW);
digitalWrite(yellowLED, LOW);
digitalWrite(blueLED, LOW);

// NETWORK CONNECTION OF SSID AND PASSWORD
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
// IP ADDRESS PRINTED
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop(){

  io.run();
  
   long state = digitalRead(hareketSensor);
  delay(1500);
    if(state == HIGH){

      Serial.println(" MOTION DETECTED!!"); //IT WORKS IF MOTION CATCHES
       digitalWrite(buzzer , HIGH);
    delay(150);
          digitalWrite(buzzer , LOW);
    }
    else {
   
      Serial.println("NO MOVE");
      }
      
WiFiClient client = server.available(); // Listens to coming clients

if (client) { // yeni client bağlanırsa
Serial.println("New Client."); //Gives message if new client connects
String currentLine = ""; // created to hold data from the client
currentTime = millis();
previousTime = currentTime;
while (client.connected() && currentTime - previousTime <= timeoutTime) { //Loop after client connects
currentTime = millis(); 
if (client.available()) { 
char c = client.read(); 
Serial.write(c); 
header += c;
if (c == '\n') { 
if (currentLine.length() == 0) {


client.println();


if (header.indexOf("GET /2/on") >= 0) { //If on is done, turn on the Red led
outputRedState = "on"; digitalWrite(redLED, HIGH);
Serial.println("SOL KIRMIZI");
count++;
counter->save(count);

} else if (header.indexOf("GET /2/off") >= 0) {
outputRedState = "off"; digitalWrite(redLED, LOW);

} else if (header.indexOf("GET /4/on") >= 0) {
outputGreenState = "on"; digitalWrite(greenLED, HIGH);
Serial.println("SARI");
count2++;
counter2->save(count2);

} else if (header.indexOf("GET /4/off") >= 0) {
outputGreenState = "off"; digitalWrite(greenLED, LOW);

} else if (header.indexOf("GET /5/on") >= 0) {
outputYellowState = "on"; digitalWrite(yellowLED, HIGH);
Serial.println("BEYAZ");
count3++;
counter3->save(count3);

} else if (header.indexOf("GET /5/off") >= 0) {
outputYellowState = "off"; digitalWrite(yellowLED, LOW);
}

else if (header.indexOf("GET /6/on") >= 0) {
outputBlueState = "on"; digitalWrite(blueLED, HIGH);
Serial.println("SAG KIRMIZI");
count4++;
counter4->save(count4);

} else if (header.indexOf("GET /6/off") >= 0) {
outputBlueState = "off"; digitalWrite(blueLED, LOW);
}

// HTML  STRUCTURE
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
// CSS to style the on/off buttons 
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");

client.println(".buttonRed { background-color: #ff0000; border: none; height:50%; width: 50%; color: white;  padding: 16px 40px; ");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");

client.println(".buttonGreen { background-color: #00ff00; border: none;  height:50%; width: 50%; color: white; padding: 16px 40px; ");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");

client.println(".buttonYellow { background-color: #feeb36; border: none;  height:50%; width: 50%; color: white; padding: 16px 40px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");

client.println(".buttonBlue { background-color: #0000FF; border: none;  height:50%; width: 50%; color: white; padding: 16px 40px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");

client.println(".buttonOff { background-color: #77878A; border: none;  height:50%; width: 50%; color: white; padding: 16px 40px; ");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");

// Title of Web Page
client.println("<body><h1>LAUNDRY CONTROL SYSTEM</h1>");

// ON/OFF for MACHINE 1
client.println("<p>Makine 1 " + outputRedState + "</p>");
if (outputRedState=="off") {// If it turns off
client.println("<p><a href=\"/2/on\"><button class=\"button buttonOff\">EMPTY</button></a></p>");
} else {
client.println("<p><a href=\"/2/off\"><button class=\"button buttonRed\">FULL</button></a></p>");
} 

// ON/OFF for MACHINE 2
client.println("<p>Makine 2 " + outputGreenState + "</p>"); 
if (outputGreenState =="off") { // If it turns off
client.println("<p><a href=\"/4/on\"><button class=\"button buttonOff\">EMPTY</button></a></p>");
} else {
client.println("<p><a href=\"/4/off\"><button class=\"button buttonGreen\">FULL</button></a></p>");
}
client.println("</body></html>");

// ON/OFF for MACHINE 3
client.println("<p>Makine 3 " + outputYellowState + "</p>");
if (outputYellowState =="off") { //If it turns off
client.println("<p><a href=\"/5/on\"><button class=\"button buttonOff\">EMPTY</button></a></p>");
} else {
client.println("<p><a href=\"/5/off\"><button class=\"button buttonYellow\">FULL</button></a></p>");
}
client.println("</body></html>");

//ON/OFF for MACHINE 4
client.println("<p>Makine 4 " + outputBlueState + "</p>"); 
if (outputBlueState =="off") { // If it turns off
client.println("<p><a href=\"/6/on\"><button class=\"button buttonOff\">EMPTY</button></a></p>");
} else {
client.println("<p><a href=\"/6/off\"><button class=\"button buttonBlue\">FULL</button></a></p>");
}
client.println("</body></html>");

// The HTTP response ends with another blank line
client.println();
// Break out of the while loop
break;
} else { // if you got a newline, then clear currentLine
currentLine = "";
}
} else if (c != '\r') { // if you got anything else but a carriage return character,
currentLine += c; // add it to the end of the currentLine
}
}
}
// Clear the header variable
header = "";
// Close the connection
client.stop();
Serial.println("");
}
}
