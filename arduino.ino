#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 7);



void setup() {
  //pinMode(buzzer, OUTPUT);
  //pinMode(smokeA0, INPUT);
  mySerial.begin(9600);   // Setting the baud rate of GSM Module 
  Serial.begin(9600);
  Serial.println("GSM Sim800A BEGIN");
  delay(100);
}

void loop() {
  //int analogSensor = analogRead(smokeA0);

 //Serial.print("Pin A0: ");
  //Serial.println(analogSensor);
  // Checks if it has reached the threshold value
 // if (analogSensor > sensorThres)
  //{
  //  tone(buzzer, 1000, 200);
 //   digitalWrite(ledPin, HIGH);
  //}
 // else
 // {
 //   noTone(buzzer);
  //  digitalWrite(ledPin, LOW); 
 // }
//  delay(100);
 

   SendMessage();
   delay(10000);

  // if (mySerial.available()>0)
    // Serial.write(mySerial.read());

}

 void SendMessage()


{

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode

  delay(1000);  // Delay of 1000 milli seconds or 1 second

  mySerial.println("AT+CMGS=\"+94715365126\"\r"); // Replace x with mobile number

  delay(1000);

  mySerial.println("I am SMS from GSM Module");// The SMS text you want to send

  delay(100);

  mySerial.println((char)26);// ASCII code of CTRL+Z

  delay(1000);

}

 /*void RecieveMessage()

{

  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS

  delay(1000);

}

*/
