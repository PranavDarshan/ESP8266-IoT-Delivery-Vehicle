#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Servo.h>  // Include the Servo library

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// Insert your network credentials
#define WIFI_SSID "replace"
#define WIFI_PASSWORD "replace"

// Insert Firebase project API Key
#define API_KEY "replace"

// Insert RTDB URL
#define DATABASE_URL "replace"

// Define servo pins
#define servo1 D2
#define servo2 D5
#define servo3 D1
#define servo4 D5

// Define motor control pins
#define ENA D6
#define IN1 D7
#define IN2 D8
#define IN3 D3
#define IN4 D0
#define ENB D4
// Define the Firebase Data object
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
// Define a Servo objects
Servo mservo1, mservo2, mservo3, mservo4;

// Initialize joystick values
int x = 50;
int y = 50;
int Speed = 255;

void setup() {
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Sign up
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Assign the callback function for the long running token generation task



  // Attach servos
  mservo1.attach(servo1);
  mservo2.attach(servo2);
  mservo3.attach(servo3);
  mservo4.attach(servo4);

  // Set motor pins as output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  delay(1000);
}

void loop() {
  // Retrieve servo positions from Firebase
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 150 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    if(Firebase.RTDB.getInt(&firebaseData, "/s1")){
  if (Firebase.RTDB.getInt(&firebaseData, "/servo1")) {
    mservo1.write(firebaseData.intData());
  }}
   if(Firebase.RTDB.getInt(&firebaseData, "/s1")){
  if (Firebase.RTDB.getInt(&firebaseData, "/servo2")) {
    mservo2.write(firebaseData.intData());
  }
   }
    if(Firebase.RTDB.getInt(&firebaseData, "/s3")){
  if (Firebase.RTDB.getInt(&firebaseData, "/servo3")) {
    mservo3.write(firebaseData.intData());
  }
    }
  if(Firebase.RTDB.getInt(&firebaseData, "/s4")){
  if (Firebase.RTDB.getInt(&firebaseData, "/servo4")) {
    mservo4.write(firebaseData.intData());
  }
  }

  // Retrieve joystick values from Firebase
  if (Firebase.RTDB.getInt(&firebaseData, "/joystick/x")) {
    x = firebaseData.intData();
    Serial.print(x);
  }

  if (Firebase.RTDB.getInt(&firebaseData, "/joystick/y")) {
    y = firebaseData.intData();
    Serial.print(y);
  }
  
  // Control the car based on joystick values
  }
  if (y > 70) {
    carForward();
    Serial.println("carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } else if (x < 30) {
    carLeft(); 
    Serial.println("carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carstop");
  }
}

void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carLeft() {
  analogWrite(ENA, 0);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
