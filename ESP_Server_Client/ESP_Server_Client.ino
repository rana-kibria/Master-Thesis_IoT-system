#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
SoftwareSerial custom_serial(D6,D5);    //Rx,Tx


int person_count=0;


// ===========Optional_Part==================
const long timeoutTime = 2000;
//Set your Static IP address
IPAddress local_IP(192, 168, 10, 20);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
//============================================

void handleRoot();              // function prototypes for HTTP handlers
void handleLogin();
void handleNotFound();
void locateitem();
void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  //delay(10);
  custom_serial.begin(9600);
  Serial.println('\n');

  //=======================================================================
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  else{
    Serial.println("STA Successfully configure");
  }
  //=======================================================================
  
//hyperparameter
  wifiMulti.addAP("FAST", "hyperparameter");   // add Wi-Fi networks you want to connect to
  
  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/login", HTTP_POST, handleLogin); // Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.on("/login", HTTP_GET, handleLogin);
  server.on("/locateitem", HTTP_GET, locateitem);
  server.on("/count", HTTP_GET, getCount);
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                            // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                     // Listen for HTTP requests from clients
}

void handleRoot() {                          // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"username\" placeholder=\"Username\"></br><input type=\"password\" name=\"password\" placeholder=\"Password\"></br><input type=\"submit\" value=\"Login\"></form><p>Try 'John Doe' and 'password123' ...</p>");
}

void locateitem(){
        //when pin number is recieved . change its status.
        String Pin    = "";
        String Status = "";
        String message=String("");
        Pin    = server.arg("pin");//.toInt();
        Status = server.arg("status");
        if (Pin!="" && Status!=""){
          if(Pin.length()==1){Pin=String("0")+String(Pin);}
          message= String("recieved_request:")+String(Pin)+String(Status);
          char varChar[message.length()];
          for (int i=0;i<message.length();i++){varChar[i]=message[i];}
          Serial.println(message);
          custom_serial.write(varChar);     //passed the pin number to the arduino using serial pin to toggle.   
        }
        
        //if message is recieved "reply_from_arduino:pinNumber,Status,personcount"
        delay(500);
        if (custom_serial.find("reply_from_arduino:")){
            String from_arduino=custom_serial.readString();
            Serial.println(from_arduino);
            //String PinNumber  =from_arduino.substring(0,2);
            String PinStatus  =from_arduino.substring(0,1);
            String PersonCount=from_arduino.substring(1,3);
            person_count      =PersonCount.toInt();
            //return to esp8266,to send data to server as jsone format
            DynamicJsonDocument jsone_object(512);
            jsone_object["pin"]    = Pin;
            jsone_object["status"] = PinStatus;//0 or 1
            jsone_object["count"]  = person_count;
            String buf;
            serializeJson(jsone_object, buf);
            server.send(200, "application/json", buf);
       }
}
void getCount(){
  if( server.hasArg("personcount")){
            if (custom_serial.find("reply_from_arduino:")){
            String from_arduino=custom_serial.readString();
            person_count=from_arduino.substring(3,5).toInt();
            }
            DynamicJsonDocument jsone_object(512);
            jsone_object["count"]  = person_count;
            String buf;
            serializeJson(jsone_object, buf);
            server.send(200, "application/json", buf);
  }
}
void handleLogin() {                         // If a POST request is made to URI /login
  if( ! server.hasArg("username") || ! server.hasArg("password") 
      || server.arg("username") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  if(server.arg("username") == "asad" && server.arg("password") == "1234") { // If both the username and the password are correct
    server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>Login successful</p>");
  } else {                                                                              // Username and password don't match
    server.send(401, "text/plain", "401: Unauthorized");
  }
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
