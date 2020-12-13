#include <SoftwareSerial.h>
SoftwareSerial my_serial(5,6);  // (Rx, Tx)
const int ledPin =  LED_BUILTIN;
//data in pins to alarm 
const int P1 = 9;
const int P2 = 10;
const int P3 = 11;
const int P4 = 12;
//Person Counter===========
int irPin1=7;
int irPin2=8;
int count=0;
boolean state1 = true;
boolean state2 = true;
boolean insideState = false;
boolean outsideIr=false;
boolean isPeopleExiting=false;
int i=1;
int State=0;

//=========================

int data=0;
void setup() {
my_serial.begin(9600);
Serial.begin(115200);

pinMode(ledPin, OUTPUT);
pinMode(P1, OUTPUT);
pinMode(P2, OUTPUT);
pinMode(P3, OUTPUT);
pinMode(P4, OUTPUT);
//for reading from IR Sensors
pinMode(irPin1, INPUT);
pinMode(irPin2, INPUT);
}

int Personcount(){
  
  if (!digitalRead(irPin1) && i==1 && state1){
     outsideIr=true;
     delay(100);
     i++;
     state1 = false;
  }

   if (!digitalRead(irPin2) && i==2 &&   state2){
     Serial.println("Entering into room");
     outsideIr=true;
     delay(100);
     i = 1 ;
     count++;
     //Serial.print("No of persons inside the room: ");
     //Serial.println(count);
     state2 = false;
  }

   if (!digitalRead(irPin2) && i==1 && state2 ){
     outsideIr=true;
     delay(100);
     i = 2 ;
     state2 = false;
  }

  if (!digitalRead(irPin1) && i==2 && state1 ){
     Serial.println("Exiting from room");
     outsideIr=true;
     delay(100);
     count--;
       Serial.print("No of persons inside the room: ");
       if(count<0){
        count=0;
       }
       Serial.println(count);
     i = 1;
     state1 = false;
  }  

    if (digitalRead(irPin1)){
     state1 = true;
    }

     if (digitalRead(irPin2)){
     state2 = true;
    }
  return count;
}

//=============================================================
void loop() {
  
  
if (my_serial.available()){
    
    if (my_serial.find("recieved_request:")){
      String from_esp= my_serial.readString();  //String("recieved_request:")+String(Pin)+String(Status);
      String PinNumber  =from_esp.substring(0,2);
      int State  =from_esp.substring(2,3).toInt();
      int pin = PinNumber.toInt();
      
      digitalWrite(pin,State);
      int sts = digitalRead(pin);
      //reply_from_arduino:pinNumber,Status,personcount"
      String reply=String("reply_from_arduino:")+String(sts)+String(Personcount());
      
      Serial.println(reply);
      Serial.println(Personcount());
      char varChar[reply.length()];
      for (int i=0;i<reply.length();i++){varChar[i]=reply[i];}
      my_serial.write(varChar);
      }
  }

}
