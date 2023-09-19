/*
sem04 ind project - antitheft alarm system using gsm 
*/
// defines pins numbers
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 7);


unsigned long startTime;

int green_led = 4;
int red_led = 3;
int buzzer = 2;

 // middle pin give the detection of any movement // past value
int pirsensor = 8;// current value // need to change for 12 digital pin as overlap with rx


const int trigPin1 = 9;
const int echoPin1 = 10;
const int trigPin2 = 5;
const int echoPin2 = 6;

int count = 0;
int pir_count = 0;

int initial_distance1=0;
int initial_distance2=0;
int mean_1 = 0;
int mean_2 = 0;
// defines variables

long duration;
int distance;



int get_distance(int trig,int echo){
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  // Calculating the distance
  if(duration * 0.034 / 2<1000){
    distance = duration * 0.034 / 2;
  }
  // Prints the distance on the Serial Monitor
  return distance;
}

void print_distance(int trig,int echo){
  Serial.print("Distance is : ");
  Serial.println(get_distance(trig,echo));
}

boolean is_theft(){
  if( (abs(initial_distance1-get_distance(trigPin1,echoPin1)) > 10 ) || (abs(initial_distance2-get_distance(trigPin2,echoPin2)) > 10 ) ){
    return true;
  }
  else{
    return false;
  }
}

int get_mean_1(int num){
   mean_1+=num;
   mean_1 = mean_1/2;
   return mean_1;
}

int get_mean_2(int num){
   mean_2+=num;
   mean_2 = mean_2/2;
   return mean_2;
}

void start_Green_LED() {
  digitalWrite(green_led, HIGH);  // turn the LED on (HIGH is the voltage level)
}

void start_red_LED() {
  digitalWrite(red_led, HIGH);  // turn the LED on (HIGH is the voltage level)                      // wait for a second
}

void stop_red_LED(){
  digitalWrite(red_led, LOW);   // turn the LED off by making the voltage LOW
}

void stop_Green_LED(){
  digitalWrite(green_led, LOW);   // turn the LED off by making the voltage LOW
}

void setup() {

  

  pinMode(pirsensor, INPUT);

  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  mySerial.begin(9600);
  Serial.println("GSM Sim800A BEGIN");
  
  delay(1000);
}

void loop() {
 


 print_distance(trigPin1,echoPin1);
 if(count==0){
   Serial.println("Starting");
   start_Green_LED();
   start_red_LED();
   delay(6000);
   count=1;
   Serial.println("Starting");
   Serial.println("GSM Sim800A BEGIN");
   SendMessage(0);
   delay(10000);
 } 

 else if(count==1){
  for(int i=0;i<20;i++){
    start_red_LED();
    start_Green_LED();
    Serial.print("Setting_up : ");
    Serial.println(i);
    initial_distance1 = get_mean_1(get_distance(trigPin1,echoPin1));
    initial_distance2 = get_mean_2(get_distance(trigPin2,echoPin2));
    //print_distance(trigPin1,echoPin1);
    delay(100);
    stop_red_LED();
    stop_Green_LED();
    delay(100);
  }

  count=2;
  Serial.print("---------------Initialize distance are ---------------");
  Serial.print("Sensor 1 : ");
  print_distance(trigPin1,echoPin1);
  start_Green_LED();
  Serial.print("Sensor 2 : ");
  print_distance(trigPin2,echoPin2);
 }
 else{
  pir_detect();
  if(is_theft()){
    Serial.println("Theft detection!!!");
    start_red_LED();
    //delay(2000);
  }
  if(get_distance(trigPin1,echoPin1)<10 | get_distance(trigPin2,echoPin2)<10){
    //Serial.println("Someone is very near the module!!!");
    SendMessage(1);
    start_red_LED();
    delay(200);
  }
  if(!(is_theft()) & !pir_detect() ){
    stop_red_LED();
  }
  if(pir_detect() ){
    //Serial.println("POSSIBLE DETECTION OF THEFT!!!@@@@@");
    SendMessage(2);
    digitalWrite(buzzer, HIGH);
    delay(1000);
    //start_red_LED();
  }
  if(!pir_detect() ){
    digitalWrite(buzzer, LOW);
  }
  }
  if (mySerial.available()>0)
     Serial.write(mySerial.read());
}

boolean pir_detect(){
  startTime = millis();
  int val = digitalRead(pirsensor);
  Serial.print("pirsensor : ");
  Serial.println(val);
 
  //delay(500);
  if(val){
    pir_count+=1;
  }
  unsigned long currentTime = millis(); // Get the current time
  unsigned long elapsedTime = currentTime - startTime; // Calculate the elapsed time
  if(pir_count>2 & elapsedTime<2000){
    //Serial.println("SERIOUS MOTION DETECTED!!!");
    //delay(1000);
    SendMessage(3);
    pir_count=0;
    return true;
    
  }
   
  return false;
}

void SendMessage(int i)
{

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode

  delay(1000);  // Delay of 1000 milli seconds or 1 second

  

  mySerial.println("AT+CMGS=\"+94715365126\"\r"); // Replace x with mobile number

  delay(1000);





  //mySerial.println("I am SMS from GSM Module");// The SMS text you want to send
  switch(i)
    {
      case 0:
        
        mySerial.println("Skull detection system activated");
        Serial.println("message send");
        delay(1000);
        break;
      case 1:
        
        mySerial.println("Someone is very near the module!!!");
        break;
      case 2:
        
        mySerial.println("POSSIBLE DETECTION OF THEFT!!!");
        break;
      case 3:
        
        mySerial.println("SERIOUS MOTION DETECTED!!!");
        break;
    }





  delay(100);

  mySerial.println((char)26);// ASCII code of CTRL+Z

  delay(1000);
  Serial.println("test ");
  delay(10000);
}
