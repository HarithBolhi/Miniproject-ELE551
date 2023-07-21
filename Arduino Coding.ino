#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h> 

SoftwareSerial mySerial(A0,A1); // Rx, Tx
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 8 //define green LED pin
#define LED_R 7 //define red LED

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name

#define Trig_PIN 6
#define Echo_PIN 5

long duration;
int distance;
int safetyDistance;
int Incoming_value = 0; 


 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  mySerial.begin(9600);  
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(3); //servo pin
  myServo.write(0); //servo start position
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(Trig_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(Echo_PIN, INPUT); // Sets the echoPin as an Input
  Serial.println("Put your card to the reader...");
  Serial.println();

}


void loop() 
{
// Clears the trigPin
  digitalWrite(Trig_PIN, LOW);
  delay(2);

// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(Trig_PIN, HIGH);
  delay(10);
  digitalWrite(Trig_PIN, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(Echo_PIN, HIGH);

// Calculating the distance
  distance= duration*0.034/2;
  
  safetyDistance = distance;
  if (safetyDistance <= 5){ //in cm 
    digitalWrite(LED_G, HIGH);
    Incoming_value = 1; 
  }
  else{
    digitalWrite(LED_G, LOW);
    Incoming_value = 0; 
  }
  mySerial.print(Incoming_value);
  delay (5000);

// Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "83 23 38 BB" ||content.substring(1) == "AA 79 C2 8C") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    delay(300);
    myServo.write(180);
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    delay(1000);
    digitalWrite(LED_R, LOW);
 
  }
  delay (5000);
}
