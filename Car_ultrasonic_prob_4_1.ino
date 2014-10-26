/*
 HC-SR04 Ping distance sensor]
 VCC to arduino 5v GND to arduino GND
 Echo to Arduino pin 13 Trig to Arduino pin 12
 pins: 23,25,27,29 - output motor relays,pin 10-Start button,
 pins:4,5,6,7,8,9-control LCD,pin 11-free
 More info at: http://goo.gl/kJ8Gl
 */
#include <LiquidCrystal.h>
int backLight = 10;    // pin 10 will control the backlight
//lcd(RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(8,9,4,5,6,7);
int numRows = 2;
int numCols = 16;
#define  log;
const int pingPin = 13;  
//pin which delivers time to receive echo using pulseIn()
int inPin = 12;
//range in cm which is considered safe to enter, anything 
//coming within less than 5 cm triggers red LED
const int RELAY1=23;
const int RELAY2=25;
const int RELAY3=27;
const int RELAY4=29;
volatile int state=0;
volatile int mark=0;
volatile int safeZone = 100; 
volatile long duration, cm;
void setup() {
  cli(); //Stop interrupt
   pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
// initialize serial communication
  Serial.begin (9600);
  //initializing the pin states
  pinMode(pingPin, OUTPUT);
  pinMode(inPin, INPUT);
  pinMode(RELAY1,OUTPUT);
  pinMode(RELAY2,OUTPUT);
  pinMode(RELAY3,OUTPUT);
  pinMode(RELAY4,OUTPUT);
// initialize Timer1 
 TCCR1A = 0; // set entire TCCR1A register to 0
 TCCR1B = 0; // same for TCCR1B
 TCNT1 = 0; //initialize counter value to 0
 // set compare match register to 1Hz timer count:
 OCR1A = 15640; // = (16*10^6)/(1*1024)-1 (must be <65536)
// turn on CTC mode:
 TCCR1B |= (1 << WGM12);
// Set CS10 and CS12 bits for 1024 prescaler:
 TCCR1B |= (1 << CS10)|(1 << CS12);
// enable timer compare interrupt:
 TIMSK1 |= (1 << OCIE1A);
sei(); // enable all interrupts 
}//end setup

volatile long microsecondsToCentimeters(long microseconds)
  {
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
  }
ISR(TIMER1_COMPA_vect) // timer compare interrupt 10HZ service routine
{
 //raw duration in milliseconds, cm is the 
//converted amount into a distance
long duration, cm;
//sending the signal, starting with LOW for a clean signal
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(5);
digitalWrite(pingPin, LOW);
//setting up the input pin, and receiving the duration in 
//microseconds for the sound to bounce off the object infront
pinMode(inPin, INPUT);
duration = pulseIn(inPin, HIGH);
cm = microsecondsToCentimeters(duration); 
// convert the time into a distance      
//printing the current reader serial display
lcd.clear();
lcd.setCursor(0,0);
lcd.print(cm);
lcd.print("cm");
lcd.println(); 
if (cm <=safeZone)
  {
    state==1;
    goto log;
  }
  else
  {
    state==0;
  }

}     
void loop()
{
 while ((state==0) && (mark==1))
     {
      digitalWrite(RELAY1, HIGH);   //moving car forward
      delay(3000);
      digitalWrite(RELAY1, LOW);   //Stop moving car forward
      digitalWrite(RELAY4, HIGH); //Turn wheels left
      delay(700);
      digitalWrite(RELAY4, LOW);   //Stop turn weels left
      delay(700);
      digitalWrite(RELAY1, HIGH);  //Move car forward
      delay(1000);
      digitalWrite(RELAY3, HIGH);  //Turn wheels right
      delay(700);
      digitalWrite(RELAY3, LOW);  //Stop turn wheels right
      delay(3000);
      digitalWrite(RELAY1, LOW);  //Stop move car forwar
      delay(2000);
      digitalWrite(RELAY3, HIGH);  //Turn wheels right
      delay(700);
      digitalWrite(RELAY3, LOW);  //Stop turn wheels right
      digitalWrite(RELAY2, HIGH);   //moving car back
      delay(2000);
      digitalWrite(RELAY4, HIGH); //Turn wheels left
      delay(700);
      digitalWrite(RELAY4, LOW);   //Stop turn weels left
      digitalWrite(RELAY1, HIGH);  //Move car forward
      delay(1000); 
      digitalWrite(RELAY3, HIGH);  //Turn wheels right
      delay(700);
      digitalWrite(RELAY3, LOW);  //Stop turn wheels right
      delay(2000);
      digitalWrite(RELAY1, LOW);  //Stop move car forwar 
    
     }
     log:
     if (mark==1)
     {
     mark==0;
     digitalWrite(RELAY1, LOW); //STOP moving car forward
     digitalWrite(RELAY2, LOW); //STOP moving car back
     digitalWrite(RELAY3, LOW); //STOP turn wheels right
     digitalWrite(RELAY4, LOW); //STOP turn wheels left
     delay(3000);
     digitalWrite(RELAY2, HIGH);  //Move car back
     delay(3000);
     digitalWrite(RELAY2, LOW);
     digitalWrite(RELAY3, HIGH); //Turn wheels right
     delay(1000);
     digitalWrite(RELAY3, LOW);   //Stop turn weels right
     delay(700);
     digitalWrite(RELAY1, HIGH);  //Move car forward
     delay(1000);
     digitalWrite(RELAY4, HIGH);  //Turn wheels left
     delay(700);
     digitalWrite(RELAY4, LOW);  //Stop turn wheels left
     delay(2000);
     digitalWrite(RELAY1, LOW);  //Stop move car forward
     mark==1;
      
     }
}
 
