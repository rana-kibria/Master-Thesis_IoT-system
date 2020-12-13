/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
int IR1 = 11;
int IR2 = 12;
int LED1  = 3;
int LED2  = 4;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  int sensor1=digitalRead(IR1);
  int sensor2=digitalRead(IR2);
  if (sensor1==HIGH){
    digitalWrite(LED1, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else{
    digitalWrite(LED1, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  
  if (sensor2==HIGH){
    digitalWrite(LED2, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else{
    digitalWrite(LED2, LOW);   // turn the LED on (HIGH is the voltage level)
  }                      // wait for a second
  //delay(1000);
}
